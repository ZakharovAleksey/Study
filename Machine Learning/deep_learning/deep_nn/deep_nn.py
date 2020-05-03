# from enum import IntEnum

import sys
sys.path.append('.')

import numpy as np
from sklearn.metrics import (accuracy_score, roc_auc_score)
from enum import IntEnum

import common_utils.my_datasets as my_ds
from common_utils.visualization import plot_scatter_with_decision_boundary

from common_utils.activations import *

from nn_layer import Layer
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
                 batch_size: int,                           # Number of objects in the training batch
                 hidden_type: LayerType = LayerType.RELU,   # Type of hidden layers
                 regularization_coef: float = 0.,           # Regularization coef
                 use_dropout: bool = False,                 # Use dropout or not
                 use_adam: bool = False,                    # Use Adam GD algorithm or not
                 beta0: float = 0.9,                        # Adam GD first momentum coef
                 beta1: float = 0.999,                      # Adam GD second momentum coef
                 use_adaptive_lr: bool = False,             # Flag for adpative learning rate usage
                 random_state: int = 1 \
        ):
        self.n_layers = len(layer_dims) - 1
        self.n_hidden = self.n_layers - 1
        self.adaptive_lr = use_adaptive_lr
        self.lr = learning_rate
        self.max_epoch = max_epoch
        self.h_type = hidden_type
        self.threshold = 0.5
        self.batch_size = batch_size

        # Initialize parameters for Adam GD
        self.use_adam = use_adam
        self.beta0 = beta0
        self.beta1 = beta1

        # Initialize NN layers
        self.layers = [
            Layer(ID, layer_dims[ID - 1], layer_dims[ID], self.h_type, \
            self.use_adam, self.beta0, self.beta1, RANDOM_STATE)
            for ID in range(1, self.n_layers)
        ]

        self.layers.append(
                Layer(self.n_layers, layer_dims[self.n_layers - 1], layer_dims[self.n_layers], \
                    LayerType.SIGM, self.use_adam, self.beta0, self.beta1, RANDOM_STATE)
            )

        # Initialize L2 regularization parameters
        self.rc = regularization_coef
        self.use_reg = self.rc != 0.

        # Initialize dropout parameters
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

    def __split_on_batches(self, X_train: np.array, y_train: np.array):
        """
            Splits input traininig data of batches
        """

        n_obj = X_train.shape[1]
        # In case we have only one batch
        if self.batch_size >= n_obj:
            self.batch_size = n_obj
            return X_train
        # In case we have several batches

        # Determine number of batches
        n_batches = 0
        if n_obj / self.batch_size == 0:
            n_batches = int(n_obj / self.batch_size)
            return np.split(X_train, n_batches, axis=0), \
                np.split(y_train, n_batches, axis=0)
        else:
            n_batches = int(np.floor(n_obj / self.batch_size))
            last_id = n_batches * self.batch_size
            # Split X_train on batches
            X_batches = np.split(X_train[:, :last_id], n_batches, axis=1)
            X_batches.append(X_train[:, last_id:])
            # Split y_train on batches
            y_batches = np.split(y_train[:, :last_id], n_batches, axis=1)
            y_batches.append(y_train[:, last_id:])

            return X_batches, y_batches

    def _run_one_epoch(self, cur_epoch: int, X_train: np.array,
                       y_train: np.array, n_obj: int):
        """
            Runs single training epoch for input training data
        """
        A_last = self._forward_propagation(X_train, self.use_dpt)
        loss = self._calculate_loss(A_last, y_train, n_obj)
        self._backward_propagation(A_last, y_train, n_obj, cur_epoch)
        self._update_weights(cur_epoch)
        return loss

    def fit(self, X_train: np.array, y_train: np.array):
        """
            Functions which fits the model
        """
        assert X_train.shape[0] == self.layers[0].W.shape[1]

        X_list, y_list = self.__split_on_batches(X_train, y_train)

        for cur_epoch in range(1, self.max_epoch + 1):
            loss = 0.
            # Model training on single batch
            if not isinstance(X_list, list):
                n_obj = X_train.shape[1]
                loss = self._run_one_epoch(cur_epoch, X_train, y_train, n_obj)
            # Model training on the list of batches
            else:
                for X, y in zip(X_list, y_list):
                    n_obj = X.shape[1]
                    loss += self._run_one_epoch(cur_epoch, X, y, n_obj)
                loss /= len(X_list)

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
                        10,
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