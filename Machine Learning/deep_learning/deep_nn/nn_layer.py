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

    def __init__(
            self,
            layer_id: int,                          # ID of layer
            n_previous: int,                        # Number of neurons in the previous layer
            n_current: int,                         # Number of neurons in the current layer
            type: LayerType = LayerType.RELU,       # Activation function type
            use_adam: bool = False,                 # Flag showing use or not Adam GD
            beta0: float = 0.9,                     # Coefficient for first momentum in Adam GD
            beta1: float = 0.999,                   # Coefficient for second momentum in Adam GD
            random_state: int = 1 \
        ):
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

        # Initialize dropdown mask
        self.dpt_mask = None

        # Initialize parameters in case we use Adam GD
        self.use_adam = use_adam
        if self.use_adam:
            self.__init_adam_params(beta0, beta1)

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

    def __init_adam_params(self, beta0: float, beta1: float):
        """
            Initialization of parameters necessary for Adam Gradient Descent
            https://towardsdatascience.com/adam-latest-trends-in-deep-learning-optimization-6be9a291375c
        """
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

    def _backward_adam(self, dA_next: np.array, n_objects: int, use_regularization: bool, \
        reg_coef: float, cur_epoch: int) -> np.array:
        """
            Backward step for layer:
            1. dZ = dA * activation_deriviative(Z)
            2. dW = (dZ * A_prev) / m
            3. db = sum(dZ, axis=1) / m
            4. Calculate bias-corrected first momentum coefs
            5. Calculate bias-corrected second momentum coefs
        """
        dA_cur = self._backward_grad(dA_next, n_objects, use_regularization,
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

    def _backward_grad(self, dA_next: np.array, n_objects: int, use_regularization: bool, \
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

    def backward(self, dA_next: np.array, n_objects: int, use_regularization: bool, \
        reg_coef: float, cur_epoch: int) -> np.array:
        """
            Backward propagation step for the current layer
        """
        if self.use_adam:
            return self._backward_adam(dA_next, n_objects, use_regularization,
                                       reg_coef, cur_epoch)
        else:
            return self._backward_grad(dA_next, n_objects, use_regularization,
                                       reg_coef, cur_epoch)

    def _update_weight_adam(self, learning_rate: float):
        """
            Update weights in accordace with corrected Adam coefficients
        """
        self.W = self.W - learning_rate * \
            self.V_dw_corr / (np.sqrt(self.S_dw_corr) + self.eps)
        self.b = self.b - learning_rate * \
            self.V_db_corr / (np.sqrt(self.S_db_corr) + self.eps)

    def _update_weight_grad(self, learning_rate: float):
        """
            Update weights basing on learning rate
        """
        self.W = self.W - learning_rate * self.dW
        self.b = self.b - learning_rate * self.db

    def update_weight(self, learning_rate: float):
        """
            Weights dW & db update for the current layer
        """
        if self.use_adam:
            self._update_weight_adam(learning_rate)
        else:
            self._update_weight_grad(learning_rate)

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