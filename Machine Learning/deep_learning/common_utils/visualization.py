import numpy as np
import matplotlib.pyplot as plt


def plot_decision_boundary(model, X: np.array, y: np.array):
    # Set min and max values and give it some padding
    x_min, x_max = X[0, :].min() - 1, X[0, :].max() + 1
    y_min, y_max = X[1, :].min() - 1, X[1, :].max() + 1
    h = 0.01
    # Generate a grid of points with distance h between them
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))
    # Predict the function value for the whole grid
    Z = model(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    # Plot the contour and training examples
    plt.contourf(xx, yy, Z, cmap=plt.cm.Spectral)
    plt.title('Decision boundary', fontsize=19)
    plt.ylabel('x2', fontsize=14)
    plt.xlabel('x1', fontsize=14)
    plt.scatter(X[0, :], X[1, :], c=y, cmap=plt.cm.Spectral)


def plot_scatter_with_decision_boundary(model, X: np.array, y: np.array):

    fig = plt.figure(figsize=(9, 8))
    ax = plt.subplot(221)
    ax.scatter(X[:, 0], X[:, 1], c=y, s=50, edgecolor='k')

    plot_decision_boundary(lambda x: model.predict(x.T), X.T, y)
    plt.show()


if __name__ == "__main__":
    print('LOL')