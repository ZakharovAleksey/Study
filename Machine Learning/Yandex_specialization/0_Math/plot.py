import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from numpy.linalg import eig


def init_plotting():
    plt.rcParams['figure.figsize'] = (10, 5)
    plt.rcParams['font.size'] = 10
    plt.rcParams['font.family'] = 'fantasy'  #'Times New Roman'
    plt.rcParams['axes.labelsize'] = plt.rcParams['font.size']
    plt.rcParams['axes.titlesize'] = 1.5 * plt.rcParams['font.size']
    plt.rcParams['legend.fontsize'] = plt.rcParams['font.size']
    plt.rcParams['xtick.labelsize'] = plt.rcParams['font.size']
    plt.rcParams['ytick.labelsize'] = plt.rcParams['font.size']
    plt.rcParams['xtick.major.size'] = 3
    plt.rcParams['xtick.minor.size'] = 3
    plt.rcParams['xtick.major.width'] = 1
    plt.rcParams['xtick.minor.width'] = 1
    plt.rcParams['ytick.major.size'] = 3
    plt.rcParams['ytick.minor.size'] = 3
    plt.rcParams['ytick.major.width'] = 1
    plt.rcParams['ytick.minor.width'] = 1
    plt.rcParams['legend.frameon'] = False
    plt.rcParams['legend.loc'] = 'center left'
    plt.rcParams['axes.linewidth'] = 1
    plt.rcParams["legend.loc"] = 'best'

    # plt.gca().spines['right'].set_color('none')
    # plt.gca().spines['top'].set_color('none')
    # plt.gca().xaxis.set_ticks_position('bottom')
    # plt.gca().yaxis.set_ticks_position('left')


def plot_func():
    fig, ax = plt.subplots(1, 2)
    x = np.arange(0, 11, 1)

    plt.gca().margins(0.1, 0.1)
    ax[0].plot(x,
               5 + (x - 5)**2,
               linestyle='-',
               marker='.',
               linewidth=1,
               color='r',
               label=r'Выпуклая функция')
    ax[1].plot(x,
               5 - (x - 5)**2,
               linestyle='-',
               marker='.',
               linewidth=1,
               color='b',
               label=r'Вогнутая функция')

    for cur_ax in ax:
        handles, labels = cur_ax.get_legend_handles_labels()
        cur_ax.legend(handles, labels)
        cur_ax.grid(True)

    ax[0].set_title(u'График выпуклой функции')
    ax[1].set_title(u'График вогнутой функции')
    plt.tight_layout()
    plt.show()


def plot_eigen_vetors():
    # Obtain eigen vectors
    A = np.array([-4, 6, -3, 5]).reshape((2, 2))
    l, eigen_vector = eig(A)
    eigen_A = A.dot(eigen_vector[:, 0])

    # Obtain not eigen vectors
    random_vector = np.array([-0.3, -0.1])
    random_A = A.dot(random_vector)

    fig, ax = plt.subplots(figsize=(5, 5))

    colors = ['C{0}'.format(i) for i in range(0, 5)]
    labels = ['СВ', 'СВ после A', 'не СВ', 'не СВ после A']

    vectors = [eigen_vector[:, 0], eigen_A, random_vector, random_A]
    vectors_shape = []

    for id in range(0, len(vectors)):
        vectors_shape.append(
            ax.arrow(0,
                     0,
                     *vectors[id],
                     width=0.03,
                     head_width=0.08,
                     head_length=0.1,
                     color=colors[id],
                     alpha=0.5))

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.legend(vectors_shape, labels)
    ax.grid(True)
    ax.set_title('Вектора до и после действия оператора A')
    plt.show()


if __name__ == '__main__':
    param = str(sys.argv[1])
    init_plotting()

    if param == 'func':
        plot_func()
    elif param == 'eigen':
        plot_eigen_vetors()
    else:
        pass
