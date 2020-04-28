# from enum import IntEnum

import numpy as np
from sklearn.metrics import (accuracy_score, roc_auc_score)
from enum import IntEnum

import common_utils.my_datasets as my_ds
from common_utils.visualization import plot_scatter_with_decision_boundary
from common_utils.activations import *

RANDOM_STATE = 1
np.random.seed(RANDOM_STATE)


class Layer:
    def __init__(self,
                 layer_id: int,
                 n_previous: int,
                 n_current: int,
                 type: LayerType = LayerType.RELU,
                 random_state: int = 1):
        np.random.seed(random_state)
        self.id = layer_id
        self.type = type
        self.act, self.act_der = ACTIVATIONS[self.type]

        self.W = np.random.randn(n_current, n_previous) * 0.01
        self.dW = None
        self.b = np.zeros((n_current, 1))
        self.db = None

        #TODO:(highlught to see, because this note is VERY important)
        # Cache below is necessray for back propagation
        # 1. Z = W*A+b on CURRENT layer
        # 2. A = g(Z) on PREVIOUS layer
        # All info stores during forward propagation
        self.Z_cur = None
        self.A_prev = None

        self.dpt_mask = None

    def forward(self, A_prev: np.array) -> np.array:
        """
            Forward step for layer: 
            1. Z = W * A + b
            2. A = activation_function(Z)
            Note: important to store A_prev & Z for further back propagation
        """
        self.A_prev = A_prev
        self.Z_cur = self.W.dot(A_prev) + self.b
        return self.act(self.Z_cur)

    def backward(self, dA_next: np.array, n_objects: int,
                 use_regularization: bool, reg_coef: float) -> np.array:
        """
            Bockward step for layer:
            1. dZ = dA * activation_deriviative(Z)
            2. dW = (dZ * A_prev) / m
            3. db = sum(dZ, axis=1) / m
        """
        dZ = dA_next * self.act_der(self.Z_cur)

        self.dW = dZ.dot(self.A_prev.T) / n_objects

        if use_regularization:
            self.dW = self.dW + (reg_coef * self.W) / n_objects
        assert self.dW.shape == self.W.shape

        self.db = np.sum(dZ, axis=1, keepdims=True) / n_objects
        assert self.db.shape == self.b.shape

        dA_cur = self.W.T.dot(dZ)
        return dA_cur

    def update_weight(self, learning_rate: float):
        """
            Update weights basing on learning rate
        """
        self.W = self.W - learning_rate * self.dW
        self.b = self.b - learning_rate * self.db

    def calc_weight_norm(self) -> float:
        """
            Calculates L2 norm for weights
        """
        return np.sum(self.W**2)

    def generate_dpt_mask(self, dpt_threshold: float) -> np.array:
        """
            Generates dropdown mask for layer basing on the 
            input threshold
        """
        self.dpt_mask = (np.random.rand(self.Z_cur.shape[0], self.Z_cur.shape[1]) < dpt_threshold). \
            astype(np.int)

    def get_dpt_mask(self) -> np.array:
        return self.dpt_mask


# Direction of propagation
class PropType(IntEnum):
    FORWARD = 0,
    BACKWARD = 1


class NeuralNetwork:
    def __init__(self,
                 layer_dims: list,
                 learning_rate: float,
                 max_epoch: int,
                 hidden_type: LayerType = LayerType.RELU,
                 regularization_coef: float = 0.,
                 use_dropout: bool = False):
        self.n_layers = len(layer_dims) - 1
        self.n_hidden = self.n_layers - 1
        self.lr = learning_rate
        self.max_epoch = max_epoch
        self.h_type = hidden_type
        self.threshold = 0.5

        # Initilalize hidden layers
        self.layers = [
            Layer(ID, layer_dims[ID - 1], layer_dims[ID], self.h_type,
                  RANDOM_STATE) for ID in range(1, self.n_layers)
        ]
        # Output layer is SIGM for classification problem
        self.layers.append(
            Layer(self.n_layers, layer_dims[self.n_layers - 1],
                  layer_dims[self.n_layers], LayerType.SIGM, RANDOM_STATE))

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
        log_loss = Y * np.log(A_last) + (1. - Y) * np.log(1. - A_last)
        loss = -np.sum(log_loss) / n_obj

        # In case we add L2 regularization
        if self.use_reg:
            loss += self.__calc_regularization_norm(n_obj)

        return loss

    def _backward_propagation(self, A_last: np.array, Y: np.array, n_obj: int):
        """
            Backward propagation step for NN
        """
        dAL = -(np.divide(Y, A_last) - np.divide(1 - Y, 1 - A_last))

        for ID in reversed(range(self.n_layers)):
            dAL = self.layers[ID].backward(dAL, n_obj, self.use_reg, self.rc)

            # Use the same dropdown masks for all layers but 1
            if self.use_dpt and self.layers[ID].id != 1:
                # Note: use layer[ID-1] for shape match
                dAL = self.__dropout_activation(self.layers[ID - 1], dAL,
                                                PropType.BACKWARD)

    def _update_weights(self):
        """
            Layers weights update after backpropagation
        """
        for layer in self.layers:
            layer.update_weight(self.lr)

    def fit(self, X_train: np.array, y_train: np.array):
        """
            Functions which fits the model
        """
        assert X_train.shape[0] == self.layers[0].W.shape[1]

        n_objects = X_train.shape[1]
        for ID in range(self.max_epoch):
            A_last = self._forward_propagation(X_train, self.use_dpt)
            loss = self._calculate_loss(A_last, y_train, n_objects)
            self._backward_propagation(A_last, y_train, n_objects)
            self._update_weights()

            if ID % 1000 == 0:
                print(f'{ID}: {round(loss, 4)}')

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
                        0.05,
                        10000,
                        LayerType.TANH,
                        regularization_coef=0.01,
                        use_dropout=False)

    clf.fit(X_train.T, y_train[:, np.newaxis].T)
    y_pred = clf.predict(X_test.T)

    print(f'Accuracy: {accuracy_score(y_test, y_pred.T)}')
    print(f'AUC ROC: {roc_auc_score(y_test, y_pred.T)}')

    plot_scatter_with_decision_boundary(clf, X_train, y_train)
