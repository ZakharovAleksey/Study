import os
import numpy as np
import matplotlib.pyplot as plt

from sklearn.datasets import make_classification
from sklearn.metrics import (accuracy_score, roc_auc_score)
from sklearn.model_selection import train_test_split


class Neuron:
    def __init__(self, max_iter: int = 100, learning_rate: float = 0.01):
        self.max_iter = max_iter
        self.lr = learning_rate
        self.n_features = 0
        self.n_obj = 0
        self.clf_threshold = 0.5

        # Parameters
        self.weights = None
        self.bias = 0.
        self.loss = []

    def __sigmioid(self, z_values: np.array) -> np.array:
        return 1. / (1. + np.exp(-z_values))

    def __loss(self, y_real: np.array, y_predicted: np.array) -> float:
        return -np.mean((y_real * np.log(y_predicted) +
                         (1. - y_real) * np.log(1. - y_predicted)))

    def __grad_weights(self, X: np.array, y_real: np.array,
                       y_pred: np.array) -> np.array:
        return X.dot((y_pred - y_real).T) / self.n_obj

    def __grad_bias(self, y_real: np.array, y_pred: np.array) -> float:
        return np.mean((y_pred - y_real))

    def __initial_initialization(self) -> tuple:
        self.weights = np.zeros(self.n_features)[:, np.newaxis]
        self.bias = 0.

    def __propagate(self, X: np.array, y_real: np.array) -> tuple:
        z = self.weights.T.dot(X) + self.bias
        y_pred = self.__sigmioid(z)

        self.loss.append(self.__loss(y_real, y_pred))

        dw = self.__grad_weights(X, y_real, y_pred)
        db = self.__grad_bias(y_real, y_pred)

        return dw, db

    def __try_fit(self, X_train: np.array, y_train: np.array):
        # If number of objects for prediction is wrong
        if X_train.shape[1] != y_train.shape[1]:
            raise Exception('Input X and y train dimensions are wrong')
        self.n_features = X_train.shape[0]
        self.n_obj = X_train.shape[1]

        self.__initial_initialization()

        for _ in range(self.max_iter):
            dw, db = self.__propagate(X_train, y_train)

            self.weights = self.weights - self.lr * dw
            self.bias = self.bias - self.lr * db

    def fit(self, X_train: np.array, y_train: np.array):
        try:
            self.__try_fit(X_train, y_train)
        except Exception as e:
            print(f'Error: {str(e)}')

    def __try_predict(self, X_test: np.array) -> np.array:
        if X_test.shape[0] != self.n_features:
            raise Exception(
                'wrong number of input features in predict function')

        z = self.weights.T.dot(X_test) + self.bias
        y_pred = self.__sigmioid(z)

        return y_pred > self.clf_threshold

    def predict(self, X_test: np.array) -> np.array:
        try:
            return self.__try_predict(X_test)
        except Exception as e:
            print(f'Error: {str(e)}')


if __name__ == '__main__':
    np.random.seed(10)

    X, y = make_classification(n_samples=300,
                               n_features=2,
                               n_informative=2,
                               n_redundant=0)

    X_train, X_test, y_train, y_test = train_test_split(X,
                                                        y,
                                                        test_size=0.3,
                                                        shuffle=True,
                                                        random_state=1)

    X_train = np.array([X_train[:, 0], X_train[:, 1]])
    y_train = y_train[np.newaxis, :]

    X_test = np.array([X_test[:, 0], X_test[:, 1]])
    y_test = y_test[np.newaxis, :].flatten()

    clf = Neuron(max_iter=1000, learning_rate=0.1)
    clf.fit(X_train, y_train)

    y_res = clf.predict(X_test).flatten()

    print(f'Accuracy: {round(accuracy_score(y_test, y_res), 3)}')
    print(f'AUC ROC: {round(roc_auc_score(y_test, y_res), 3)}')

    fig, ax = plt.subplots(figsize=(10, 5))
    w0, w1 = clf.weights
    b = clf.bias

    ax.scatter(X_train[0], X_train[1], c=y_train.flatten())
    ax.plot(X_train[0], w0 * X_train[0] + b, 'k')
    plt.show()
