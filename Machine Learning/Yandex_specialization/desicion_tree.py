import numpy as np

from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier as DTC
from sklearn.metrics import f1_score


class DecisionTree:
    def __init__(self, max_depth: int = 4):
        self.max_depth = max_depth
        self.thresholds = []

    def __gini(self, n_samples: int, class_proportions: list) -> float:
        return 1. - np.sum(
            (np.array(class_proportions, dtype=float) / n_samples)**2)

    def __calc_proportion(self, feature: np.array, target: np.array,
                          mask: list, unique: np.array) -> np.array:
        split = np.zeros(unique.shape[0])
        for ID, class_ in enumerate(unique):
            split[ID] = np.sum(target[mask] == class_)
        return split

    def __split(self, feature: np.array, target: np.array,
                thresh: float) -> tuple:
        unique_classes = np.unique(y)
        left_mask = feature <= thresh
        right_mask = feature > thresh

        return (left_mask, right_mask,
                self.__calc_proportion(feature, target, left_mask,
                                       unique_classes),
                self.__calc_proportion(feature, target, right_mask,
                                       unique_classes))

    def __calc_thresholds(self, feature_vals: np.array) -> np.array:
        unique_vals = np.unique(feature_vals)
        thresholds = np.zeros(unique_vals.shape[0] + 1)
        for ID, cur_val in enumerate(unique_vals[:-1]):
            thresholds[ID] = (cur_val + unique_vals[ID + 1]) / 2.
        return thresholds

    def fit(self, X_train: np.array, y_train: np.array):
        feature_id = 3
        cur_filter = np.ones(X_train.shape[0], dtype=bool)

        x = X_train[cur_filter, feature_id]
        thresholds = self.__calc_thresholds(x)
        loses = np.zeros(thresholds.shape[0])
        for ID, cur_thr in enumerate(thresholds):
            mask_l, mask_r, prop_l, prop_r = self.__split(
                x, y_train[cur_filter], cur_thr)

            n_l = np.sum(mask_l)
            n_r = np.sum(mask_r)
            N = n_l + n_r

            gini_l = self.__gini(n_l, prop_l)
            gini_r = self.__gini(n_r, prop_r)
            loses[ID] = (n_l * gini_l + n_r * gini_r) / N
        self.thresholds.append(thresholds[np.argmin(loses)])


if __name__ == '__main__':
    data = load_iris()
    X = data.data
    y = data.target

    X_train, X_test, y_train, y_test = train_test_split(X,
                                                        y,
                                                        test_size=0.3,
                                                        shuffle=True,
                                                        stratify=y,
                                                        random_state=1)

    clf = DecisionTree()
    clf.fit(X_train, y_train)
