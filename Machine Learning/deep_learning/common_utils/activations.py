
from enum import IntEnum
from scipy.special import expit
import numpy as np

class LayerType(IntEnum):
    RELU = 0,
    SIGM = 1,
    TANH = 2


def sigmoid(Z: np.array) -> np.array:
    return expit(Z)


def relu(Z: np.array) -> np.array:
    return np.maximum(0., Z)


def sigmoid_der(Z: np.array) -> np.array:
    s = expit(Z)
    return s * (1. - s)


def relu_der(Z: np.array) -> np.array:
    return np.heaviside(Z, 1.)


def tanh(Z: np.array) -> np.array:
    return np.tanh(Z)


def tanh_der(Z: np.array) -> np.array:
    return 1. - np.tanh(Z)**2


ACTIVATIONS = {
    LayerType.RELU: (relu, relu_der),
    LayerType.SIGM: (sigmoid, sigmoid_der),
    LayerType.TANH: (tanh, tanh_der)
}