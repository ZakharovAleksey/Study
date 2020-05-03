# from enum import IntEnum

import sys
sys.path.append('.')

import numpy as np
from sklearn.metrics import (accuracy_score, roc_auc_score)
from enum import IntEnum

import common_utils.my_datasets as my_ds
from common_utils.visualization import plot_scatter_with_decision_boundary

from common_utils.activations import *

from nn_layer import Layer, AdamLayer
RANDOM_STATE = 1
np.random.seed(RANDOM_STATE)


# Direction of propagation
class PropType(IntEnum):
    FORWARD = 0,
    BACKWARD = 1


class NeuralNetwork:
    """
        This is my naive implementation of NN:

    """
    def __init__(self,
                 layer_dims: list,                          # Layer dimentions
                 learning_rate: float,                      # Start learning rate
                 max_epoch: int,                            # Max number of epochs
                 hidden_type: LayerType = LayerType.RELU,   # Type of hidden layers
                 regularization_coef: float = 0.,           # Regularization coef
                 use_dropout: bool = False,                 # Use dropout or not
                 use_adam: bool = False,                    # Use Adam GD algorithm or not
                 use_adaptive_lr: bool = False \
        ):
        self.n_layers = len(layer_dims) - 1
        self.n_hidden = self.n_layers - 1
        self.adaptive_lr = use_adaptive_lr
        self.lr = learning_rate
        self.max_epoch = max_epoch
        self.h_type = hidden_type
        self.threshold = 0.5

        # Initilalize hidden layers
        if use_adam:
            self.layers = [
                AdamLayer(ID, layer_dims[ID - 1], layer_dims[ID], self.h_type, \
                    0.9, 0.999, RANDOM_STATE) for ID in range(1, self.n_layers)
            ]
        else:
            self.layers = [
                Layer(ID, layer_dims[ID - 1], layer_dims[ID], self.h_type, \
                    RANDOM_STATE) for ID in range(1, self.n_layers)
            ]

        # Output layer is SIGM for classification problem
        if use_adam:
            self.layers.append(
                AdamLayer(self.n_layers, layer_dims[self.n_layers - 1], \
                    layer_dims[self.n_layers], LayerType.SIGM, 0.9, 0.999, RANDOM_STATE)
            )
        else:
            self.layers.append(
                Layer(self.n_layers, layer_dims[self.n_layers - 1], \
                    layer_dims[self.n_layers], LayerType.SIGM, RANDOM_STATE)
            )

        # Regularization parameters
        self.rc = regularization_coef
        self.use_reg = self.rc != 0.

        # Dropout parameters
        self.use_dpt = use_dropout
        self.dpt_thresh = 0.5

    def __dropout_activation(self, layer: Layer, A: np.array,
                             propagation_type: PropType) -> np.array:
        # Mask changes on each forward propagation iteration
        if propagation_type == PropType.FORWARD:
            layer.generate_dpt_mask(self.dpt_thresh)

        dpt_thresh_rev = 1. / self.dpt_thresh
        # Apply dropout mask and increase activation because of
        # several neurons dropout
        assert A.shape == layer.get_dpt_mask().shape
        return np.multiply(A, layer.get_dpt_mask()) * dpt_thresh_rev

    def _forward_propagation(self, X: np.array,
                             use_dropdown: bool = False) -> np.array:
        """
            Forward propagation step for NN
        """
        A = X
        for layer in self.layers:
            A = layer.forward(A)

            # Apply dropdown to each layer but the last one (1 neuron only)
            if use_dropdown and layer.id != self.n_layers:
                A = self.__dropout_activation(layer, A, PropType.FORWARD)
        return A

    def __calc_regularization_norm(self, n_obj: int):
        """
            Calculate weight L2 norm in regularization
            as an additional part of loss computation
        """
        weigth_norm = 0.
        for layer in self.layers:
            weigth_norm += layer.calc_weight_norm()
        return self.rc * weigth_norm / (2 * n_obj)

    def _calculate_loss(self, A_last: np.array, Y: np.array,
                        n_obj: int) -> float:
        """
            Loss calculation
        """
        # Handle values close to 0 and 1 (log(0) -> inf)
        A_last[A_last == 0] = 1e-6
        A_last[A_last == 1] = 0.999
        # Calculate loss
        log_loss = Y * np.log(A_last) + (1. - Y) * np.log(1. - A_last)
        loss = -np.sum(log_loss) / n_obj

        # In case we add L2 regularization
        if self.use_reg:
            loss += self.__calc_regularization_norm(n_obj)

        return loss

    def _backward_propagation(self, A_last: np.array, Y: np.array, n_obj: int,
                              cur_epoch: int):
        """
            Backward propagation step for NN
        """
        dAL = -(np.divide(Y, A_last) - np.divide(1 - Y, 1 - A_last))

        for ID in reversed(range(self.n_layers)):
            dAL = self.layers[ID].backward(dAL, n_obj, self.use_reg, self.rc,
                                           cur_epoch)

            # Use the same dropdown masks for all layers but 1
            if self.use_dpt and self.layers[ID].id != 1:
                # Note: use layer[ID-1] for shape match
                dAL = self.__dropout_activation(self.layers[ID - 1], dAL,
                                                PropType.BACKWARD)

    def _update_weights(self, cur_epoch):
        """
            Layers weights update after backpropagation
        """
        cur_lr = self.lr
        if self.adaptive_lr:
            cur_lr /= np.sqrt(cur_epoch)

        for layer in self.layers:
            layer.update_weight(cur_lr)

    def fit(self, X_train: np.array, y_train: np.array):
        """
            Functions which fits the model
        """
        assert X_train.shape[0] == self.layers[0].W.shape[1]

        n_objects = X_train.shape[1]
        for cur_epoch in range(1, self.max_epoch + 1):
            A_last = self._forward_propagation(X_train, self.use_dpt)
            loss = self._calculate_loss(A_last, y_train, n_objects)
            # print(loss)
            self._backward_propagation(A_last, y_train, n_objects, cur_epoch)
            self._update_weights(cur_epoch)

            if cur_epoch % 100 == 0:
                print(f'Epoch {cur_epoch/1000}k: {round(loss, 6)}')

    def predict(self, X_test: np.array) -> np.array:
        """
            Functions which predicts results on new data
        """
        assert X_test.shape[0] == self.layers[0].W.shape[1]

        # On prediction step we never use dropdown (that's why False)
        y_pred = self._forward_propagation(X_test, False)
        return y_pred >= self.threshold


if __name__ == "__main__":
    X_train, X_test, y_train, y_test = my_ds.my_moon_dataset()
    layer_dims = [X_train.shape[1], 5, 3, 1]

    clf = NeuralNetwork(layer_dims,
                        0.5,
                        2000,
                        LayerType.TANH,
                        regularization_coef=0.08,
                        use_dropout=False,
                        use_adam=True,
                        use_adaptive_lr=True)

    clf.fit(X_train.T, y_train[:, np.newaxis].T)
    y_pred = clf.predict(X_test.T)

    print(f'Accuracy: {accuracy_score(y_test, y_pred.T)}')
    print(f'AUC ROC: {roc_auc_score(y_test, y_pred.T)}')

    plot_scatter_with_decision_boundary(clf, X_train, y_train)
