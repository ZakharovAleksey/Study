import numpy as np
from scipy.optimize import minimize, differential_evolution
import matplotlib.pyplot as plt


# [1; 30] range
def f(x):
    return np.sin(x / 5.) * np.exp(x / 10.) + 5. * np.exp(-x / 2.)


def print_func():
    x = np.arange(0, 30.5, 0.5)

    fig, ax = plt.subplots(figsize=(10, 5))
    ax.plot(x, f(x))
    ax.grid(True)
    plt.show()


if __name__ == '__main__':

    for i in range(0, 30):
        res = minimize(f, i)
        print('{0} : {1}'.format(i, res.x))

    # Task 1
    min_1 = minimize(f, 2., method='BFGS')
    a = round(f(min_1.x)[0], 2)
    min_2 = minimize(f, 30., method='BFGS')
    b = round(f(min_2.x)[0], 2)

    with open('t_1.txt', 'w') as file:
        file.write('{0} {1}'.format(a, b))
    print_func()