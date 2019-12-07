import numpy as np
import os
import re
import collections
import matplotlib.pyplot as plt

from scipy.spatial import distance
from scipy import linalg

# Task 1. Closest centences
path = os.path.join(os.getcwd(), 'data', 'sentences.txt')


def get_data(path):
    if os.path.exists(path):
        with open(path, 'r') as f:
            return f.readlines()
    return []


def read_data(path):
    try:
        data = get_data(path)
        if not data:
            raise 'Incoming file is empty or doesn\'t exist.'
        return data
    except Exception as e:
        print('Error: {0}'.format(str(e)))


def sentence_dist():
    data = read_data(path)

    # Create matrix with words from sentence
    sentences = []
    for line in data:
        sentences.append(
            [word for word in re.split('[^a-z]', line.lower()) if word])
    # Create dictionary with all existing words
    words = set()
    for sentence in sentences:
        words = words.union(sentence)
    words_dict = {word: id for id, word in enumerate(words)}

    # Create matrix: [sentence, word]
    N_rows = len(sentences)
    N_cols = len(words_dict)
    matrix = np.zeros((N_rows, N_cols))

    for i in range(N_rows):
        for j, cur_word in enumerate(words_dict):
            matrix[i, j] = sentences[i].count(cur_word)

    # Calcualte distances between sentences
    dist = []
    for vec in matrix[1:]:
        dist.append(distance.cosine(matrix[0], vec))

    # Find two closest
    args = np.argsort(dist)
    with open('out_1.txt', 'w') as out:
        out.write('{0} {1}'.format(args[0] + 1, args[1] + 1))


# Task 2. Function approximation
def func(x):
    return np.sin(x / 5.) * np.exp(x / 10.) + 5. * np.exp(-x / 2.)


def func_approximation(is_plot):
    x = np.arange(0, 15.5, 0.5)
    y = func(x)

    # Linear
    b_0 = [func(1.), func(15.)]
    A_0 = np.array([1, 1, 1, 15], dtype=float).reshape((2, 2))
    w_0 = linalg.solve(A_0, b_0)

    # Quadric
    b_1 = func(np.array([1, 8, 15], dtype=float))
    A_1 = np.array([[1, 1, 1], [1, 8, 8**2], [1, 15, 15**2]])
    w_1 = linalg.solve(A_1, b_1)
    # Cubic
    b_2 = func(np.array([1, 4, 10, 15], dtype=float))
    A_2 = np.array([[1, 1, 1, 1], [1, 4, 4**2, 4**3], [1, 10, 10**2, 10**3],
                    [1, 15, 15**2, 15**3]])
    w_2 = linalg.solve(A_2, b_2)

    with open('out_2.txt', 'w') as out:
        out.write(' '.join(str(round(x, 2)) for x in w_2))

    # Plot
    if is_plot:
        fig, ax = plt.subplots(figsize=(10, 5))
        ax.plot(x, y, label='initial', lw=3)
        # Linear
        ax.plot(x, w_0[0] + w_0[1] * x, label='linear', lw=3)
        # Quadric
        ax.plot(x, w_1[0] + w_1[1] * x + w_1[2] * x**2, label='quadric', lw=3)
        # Cubic
        ax.plot(x,
                w_2[0] + w_2[1] * x + w_2[2] * x**2 + w_2[3] * x**3,
                label='cubic',
                lw=3)
        ax.grid(True)

        handles, labels = ax.get_legend_handles_labels()
        ax.legend(handles, labels)

        plt.show()


if __name__ == '__main__':
    func_approximation(False)
