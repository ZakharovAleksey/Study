import random
import numpy as np
from collections import Counter

from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier as DTC
from sklearn.metrics import (f1_score, roc_auc_score)


class Node:
    '''
        Single node for MyDecisionTreeClassifier
    '''
    def __init__(self):
        self.is_leaf = False
        self.thresh = np.nan
        self.feature_id = 0
        self.obj_mask = []

        self.obj_class = np.nan
        self.proportions = None
        self.impurity = 0

        self.left = None
        self.right = None

    def set_parameters(self, class_proportions: np.array, impurity: float,
                       node_obj_mask: np.array):
        '''Set additional parameters to nodes
        '''
        self.proportions = class_proportions
        self.impurity = impurity
        self.obj_mask = node_obj_mask


class MyDecisionTreeClassifier:
    '''
        My own implementation of Decision Tree classifier
    '''
    def __init__(self,
                 max_depth: int = 3,
                 min_samples_leaf: int = 3,
                 criterion: str = 'gini',
                 random_state: int = None):
        self.max_depth = max_depth
        self.root = Node()
        self.min_samples_leaf = min_samples_leaf
        self.n_features = 0
        self.impurity_func = self.__gini if criterion == 'gini' else self.__entropy
        self.rnd = np.random.RandomState(random_state)

    def __feat_id(self):
        '''Choose random ID of feature for training
        '''
        return self.rnd.randint(0, self.n_features)

    def __entropy(self, n_samples: int, class_proportions: list) -> float:
        '''Calculates Information Gain as entropy
        '''
        if not n_samples or n_samples == 0:
            return np.nan
        p = [i for i in np.array(class_proportions) / n_samples if i != 0.]
        return -np.sum(p * np.log2(p))

    def __gini(self, n_samples: int, class_proportions: list) -> float:
        '''Calculates Information Gain via Gini approach
        '''
        if not n_samples or n_samples == 0:
            return np.nan

        return 1. - np.sum(
            (np.array(class_proportions, dtype=float) / n_samples)**2)

    def __calc_proportion(self, feature: np.array, target: np.array,
                          mask: list, unique_classes: np.array) -> np.array:
        '''Calculates how many objects of each class present in target
        '''
        split = np.zeros(unique_classes.shape[0])
        for ID, uniq_class in enumerate(unique_classes):
            split[ID] = np.sum(target[mask] == uniq_class)
        return split

    def __split(self, feature: np.array, target: np.array,
                threshold: float) -> tuple:
        '''Splits objects on two classes (left & right) by threshold
        '''
        unique_classes = np.unique(target)

        left_mask = feature <= threshold
        right_mask = feature > threshold

        return (left_mask, right_mask,
                self.__calc_proportion(feature, target, left_mask,
                                       unique_classes),
                self.__calc_proportion(feature, target, right_mask,
                                       unique_classes))

    def __find_most_frequent_class(self, cur_node: Node,
                                   target: np.array) -> int:
        '''Finds most frequent class in target
        '''
        classes = list(Counter(target).keys())
        frequency = list(Counter(target).values())
        # If all classes has the same frequency
        if np.unique(frequency).shape[0] == 1:
            cur_node.obj_class = random.choice(classes)
        else:
            cur_node.obj_class = classes[np.nanargmax(frequency)]
        cur_node.is_leaf = True

    def __calc_thresholds(self, features: np.array) -> np.array:
        '''Calculates array of thresholds for chosen feature array
        '''
        unique_features = np.unique(features)
        thresholds = np.zeros(unique_features.shape[0] + 1)
        for ID, cur_val in enumerate(unique_features[:-1]):
            thresholds[ID] = (cur_val + unique_features[ID + 1]) / 2.
        return thresholds

    def __is_leaf(self, cur_node: Node, x: np.array, y: np.array,
                  depth: int) -> bool:
        '''Checks if current node is leaf
        '''
        # If depth ix max possible
        if depth == self.max_depth:
            self.__find_most_frequent_class(cur_node, y)
            return True
        # If all objects belongs to the same class
        if np.unique(y).shape[0] == 1:
            cur_node.obj_class = y[0]
            cur_node.is_leaf = True
            return True
        # If there are few objects lest in node
        if x.shape[0] <= self.min_samples_leaf:
            self.__find_most_frequent_class(cur_node, y)
            return True

        return False

    def __fill_node(self, cur_node: Node, X: np.array, y: np.array,
                    depth: int):
        '''Calculates all information for current node necessary for classification
        '''
        cur_node.feature_id = self.__feat_id()

        x = X[:, cur_node.feature_id]
        # If we get to leaf -> return
        if self.__is_leaf(cur_node, x, y, depth):
            return

        min_loss = np.Inf

        cur_node.left = Node()
        cur_node.right = Node()

        for cur_thr in self.__calc_thresholds(x):
            mask_l, mask_r, prop_l, prop_r = self.__split(x, y, cur_thr)

            nmb_obj_l = np.sum(mask_l)
            nmb_obj_r = np.sum(mask_r)
            n_total = nmb_obj_l + nmb_obj_r

            impurity_l = self.impurity_func(nmb_obj_l, prop_l)
            impurity_r = self.impurity_func(nmb_obj_r, prop_r)

            cur_loss = (nmb_obj_l * impurity_l +
                        nmb_obj_r * impurity_r) / n_total
            # Find minimum possible loss
            if cur_loss < min_loss:
                min_loss = cur_loss
                cur_node.thresh = cur_thr
                cur_node.left.set_parameters(prop_l, impurity_l, mask_l)
                cur_node.right.set_parameters(prop_r, impurity_r, mask_r)

        self.__fill_node(cur_node.left, X[cur_node.left.obj_mask, :],
                         y[cur_node.left.obj_mask], depth + 1)
        self.__fill_node(cur_node.right, X[cur_node.right.obj_mask, :],
                         y[cur_node.right.obj_mask], depth + 1)

    def fit(self, X_train: np.array, y_train: np.array):
        '''Trains model on input data
        '''
        self.n_features = X_train.shape[1]
        self.__fill_node(self.root, X_train, y_train, 0)

    def __predict_class(self, cur_node: Node, x: np.array) -> int:
        '''Predict class for feature row
        '''
        if cur_node.is_leaf:
            return cur_node.obj_class

        value = x[cur_node.feature_id]
        if value <= cur_node.thresh:
            return self.__predict_class(cur_node.left, x)
        else:
            return self.__predict_class(cur_node.right, x)

    def predict(self, X_test: np.array) -> np.array:
        '''Predicts targets for given test data
        '''
        if X_test.shape[1] != self.n_features:
            raise Exception(
                f'Number of features for prediction is invalid. Expected {self.n_features}'
            )
        return np.array([self.__predict_class(self.root, x) for x in X_test])


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

    clf = MyDecisionTreeClassifier(random_state=1)
    clf.fit(X_train, y_train)
    y_pred = clf.predict(X_test)
    print('F1 score: {0:.3f}'.format(f1_score(y_test, y_pred,
                                              average='micro')))