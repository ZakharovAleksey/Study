import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


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


if __name__ == '__main__':
    param = str(sys.argv[1])
    init_plotting()

    if param == 'func':
        plot_func()
