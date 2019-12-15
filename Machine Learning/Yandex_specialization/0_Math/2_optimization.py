import numpy as np
from scipy.optimize import minimize, differential_evolution
import matplotlib.pyplot as plt


# [1; 30] range
def f(x):
    return np.sin(x / 5.) * np.exp(x / 10.) + 5. * np.exp(-x / 2.)


def h(x):
    if isinstance(x, int):
        return int(f(x))
    else:
        return f(x).astype(int)


def print_func(func):
    x = np.arange(0, 30.5, 0.5)
    y = func(x)

    fig, ax = plt.subplots(figsize=(10, 5))
    ax.plot(x, y)
    ax.grid(True)
    plt.show()


def task_1():
    # Task 1
    min_1 = minimize(f, 2., method='BFGS')
    a = round(f(min_1.x)[0], 2)
    min_2 = minimize(f, 30., method='BFGS')
    b = round(f(min_2.x)[0], 2)

    with open('t_1.txt', 'w') as file:
        file.write('{0} {1}'.format(a, b))


def task_2():
    min_1 = differential_evolution(f, [(1, 30)])
    print(min_1.x, round(min_1.fun[0], 2))

    with open('out_2.txt', 'w') as out:
        out.write('{0}'.format(round(min_1.fun[0], 2)))


def task_3():
    min_1 = minimize(h, 30.)
    print(min_1)
    print(min_1.x, min_1.fun)

    min_2 = differential_evolution(h, [(1, 30)])
    print(min_2.x, min_2.fun)

    with open('out_3.txt', 'w') as out:
        out.write('{0} {1}'.format(round(min_1.fun[0], 2), round(min_2.fun,
                                                                 2)))


import matplotlib.pyplot as plt


class Result():
    def __init__(self, point, min_value, iter_points, iter_numb):
        self.point = point
        self.min_value = min_value
        self.iter_points = iter_points
        self.iter_numb = iter_numb

    @property
    def x(self):
        return self.point

    @property
    def fun(self):
        return self.min_value

    @property
    def n(self):
        return self.iter_numb

    @property
    def p_iter(self):
        return self.iter_points


def func(x, y):
    return np.sqrt((x - 3.)**2 + (y - 2.)**2)


def not_grad_descent(func, p_start, p_storage, d=1e-3, eps=1e-3, max_iter=1e4):
    iter_count = 0

    while True and iter_count <= max_iter:
        iter_count += 1
        # Chose random direction
        rand_dir = np.random.uniform(low=-1., high=1., size=(2, ))
        tmp = p_start + d * rand_dir
        # Calculate step
        step = (func(p_start[0], p_start[1]) - func(tmp[0], tmp[1])) / d
        # Find next iteration point
        p_end = p_start + step * rand_dir
        p_storage.append(np.array(p_end))

        # Calculate delta (maybe via f(strat) - f (end))
        delta = np.linalg.norm(p_start - p_end)
        p_start = p_end

        if delta < eps:
            break

    if iter_count == max_iter:
        print('The method did not converge')
    else:
        return Result(p_start, func(p_start[0], p_start[1]),
                      np.array(p_storage), iter_count)


# SVD
from numpy.linalg import svd, matrix_rank, norm

if __name__ == '__main__':

    A = np.array([[3, 1, 4, 2, 5], [2, 0, 2, 2, 4], [1, 1, 2, 0, 1]],
                 dtype=float)
    U, D, Vt = svd(A)
    print(matrix_rank(A))
    print(D)

    print(U)
    n = 1
    u = U[:, :n]
    d = np.diag(D[:n])
    vt = Vt[:n, :]

    print('____' * 8)
    print(A)
    print(u.dot(d.dot(vt)))
    print(norm(A - u.dot(d.dot(vt))))

    print('LOL')

    # max_val = 6.
    # vals = np.arange(-max_val, max_val, 0.01)
    # y, x = np.meshgrid(vals, vals)
    # z = func(x, y)

    # fig, ax = plt.subplots(figsize=(7, 6))

    # c = ax.pcolormesh(x, y, z, cmap='jet')
    # ax.contour(x,y, z, cmap='jet', alpha = 0.5)
    # fig.colorbar(c, ax = ax)

    # start_point = np.array([-4., -4.])
    # iter_points = [start_point]

    # res = not_grad_descent(func, start_point, iter_points)
    # print(res.x, res.n, res.fun)

    # ax.plot(res.p_iter[:, 0], res.p_iter[:, 1], 'ko', alpha=0.5, ms=5)
    # plt.show()
