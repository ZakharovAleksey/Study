import numpy as np
from sklearn.datasets import make_moons
from sklearn.model_selection import train_test_split

DATA_RANDOM_STATE = 1
np.random.seed(DATA_RANDOM_STATE)


def my_moon_dataset():
    X, y = make_moons(random_state=DATA_RANDOM_STATE, noise=0.3)
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, train_size=0.65, random_state=DATA_RANDOM_STATE)

    return X_train, X_test, y_train, y_test


if __name__ == '__main__':
    pass