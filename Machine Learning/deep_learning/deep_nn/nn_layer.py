import sys
sys.path.append('.')

import numpy as np
from common_utils.activations import *


class Layer:
    """
        Class represents a single layer in NN.
    """
    WEIGHTS_COEF = {
        LayerType.RELU: lambda denominator: np.sqrt(2. / denominator),
        LayerType.SIGM: lambda denominator: 1. / np.sqrt(denominator),
        LayerType.TANH: lambda denominator: np.sqrt(2. / denominator),
    }

    def __init__(self, layer_id: int, n_previous: int, n_current: int, \
        type: LayerType = LayerType.RELU, random_state: int = 1):
        np.random.seed(random_state)
        self.id = layer_id
        self.type = type
        self.act, self.act_der = ACTIVATIONS[self.type]

        self.W = np.random.randn(n_current, n_previous) * \
            self.__calc_weight_coef(n_previous, n_current)
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

    def __calc_weight_coef(self, n_previous: int, n_current: int) -> float:
        """
            Returns "normalization" coefficient for initial weights, 
            depending on the type of neurons in the layer.
            Note: each neuron type has its own coefficient for better SGD convergence
        """
        coef_func = self.WEIGHTS_COEF[self.type]
        if self.type in [LayerType.RELU, LayerType.TANH]:
            return coef_func(n_previous)
        else:
            return coef_func(n_previous + n_current)

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

    def backward(self, dA_next: np.array, n_objects: int, use_regularization: bool, \
        reg_coef: float, cur_epoch: int) -> np.array:
        """
            Backward step for layer:
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


class AdamLayer(Layer):
    """
        Class represents a single layer of neurons in NN.
        For Gradient Decsent (GD) this class use Adam algorithm:
        https://towardsdatascience.com/adam-latest-trends-in-deep-learning-optimization-6be9a291375c
    """
    def __init__(self, layer_id: int, n_previous: int, n_current: int, \
        type: LayerType = LayerType.RELU, beta0: float = 0.9, beta1: float = 0.999, random_state: int = 1):
        Layer.__init__(self, layer_id, n_previous, n_current, type,
                       random_state)
        self.beta0 = beta0
        self.beta1 = beta1
        # This epsilon is needed to prevent division by zero
        self.eps = 1e-8

        # Get the dimentions of W and b
        w_nx, w_ny = self.W.shape
        b_nx, b_ny = self.b.shape

        # First momentum for Gradient Gescent
        self.V_dw = np.zeros((w_nx, w_ny))
        self.V_db = np.zeros((b_nx, b_ny))
        # Bias-corrected first momentum for Gradient Gescent
        self.V_dw_corr = np.zeros((w_nx, w_ny))
        self.V_db_corr = np.zeros((b_nx, b_ny))

        # Second momentum for Gradient Gescent (RMSprop)
        self.S_dw = np.zeros((w_nx, w_ny))
        self.S_db = np.zeros((b_nx, b_ny))
        # Bias-corrected second momentum for Gradient Gescent (RMSprop)
        self.S_dw_corr = np.zeros((w_nx, w_ny))
        self.S_db_corr = np.zeros((b_nx, b_ny))

    def __calc_momentum_coef(self, coef: np.array, grads: np.array,
                             cur_epoch: int):
        """
            This functions calculates bias-corrected first momentum for 
            input gradient (moment)
        """
        coef = self.beta0 * coef + (1. - self.beta0) * grads
        return coef, coef / (1. - self.beta0**cur_epoch)

    def __calc_rmsprop_coef(self, coef: np.array, grads: np.array,
                            cur_epoch: int):
        """
            This functions calculates bias-corrected second momentum for 
            input gradient (RMSprop)
        """
        coef = self.beta1 * coef + (1. - self.beta1) * grads**2
        return coef, coef / (1. - self.beta1**cur_epoch)

    def backward(self, dA_next: np.array, n_objects: int, use_regularization: bool, \
        reg_coef: float, cur_epoch: int) -> np.array:
        """
            Backward step for layer:
            1. dZ = dA * activation_deriviative(Z)
            2. dW = (dZ * A_prev) / m
            3. db = sum(dZ, axis=1) / m
            4. Calculate bias-corrected first momentum coefs
            5. Calculate bias-corrected second momentum coefs
        """
        dA_cur = Layer.backward(self, dA_next, n_objects, use_regularization,
                                reg_coef, cur_epoch)

        # Calculate current "weights" for momenum
        self.V_dw = self.beta0 * self.V_dw + (1. - self.beta0) * self.dW
        self.V_db = self.beta0 * self.V_db + (1. - self.beta0) * self.db

        self.V_dw_corr = self.V_dw / (1. - self.beta0**cur_epoch)
        self.V_db_corr = self.V_db / (1. - self.beta0**cur_epoch)

        # Calculate current "weights" for RMSprop
        self.S_dw = self.beta1 * self.S_dw + (1. - self.beta1) * self.dW**2
        self.S_db = self.beta1 * self.S_db + (1. - self.beta1) * self.db**2

        self.S_dw_corr = self.S_dw / (1. - self.beta1**cur_epoch)
        self.S_db_corr = self.S_db / (1. - self.beta1**cur_epoch)

        return dA_cur

    def update_weight(self, learning_rate: float):
        """
            Update weights in accordace with corrected Adam coefficients
        """
        self.W = self.W - learning_rate * \
            self.V_dw_corr / (np.sqrt(self.S_dw_corr) + self.eps)
        self.b = self.b - learning_rate * \
            self.V_db_corr / (np.sqrt(self.S_db_corr) + self.eps)
