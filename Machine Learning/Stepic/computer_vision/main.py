import numpy as np
import torch


def get_grad():
    # Create tensor! required_grad - is necessary field for further gradient
    X = torch.tensor(np.array([5, 10, 1, 2], dtype=float).reshape((2, 2)), requires_grad=True)
    # Function for which we take gradient
    function = torch.log(torch.log(X + 7.)).prod()
    # Calculate gradient
    function.backward()
    # Gradient is stored in tensor
    print(X.grad)

def gradient_descent():
    X = torch.tensor(np.array([5, 10, 1, 2.]).reshape((2,2)), requires_grad=True)
    alpha = 0.001
    
    for _ in range(500):
         # it's critical to calculate function inside the loop:
        func = torch.log(torch.log(X + 7.)).prod()
        func.backward()
        X.data -= alpha * X.grad
        X.grad.zero_()

    print(X)


def gradient_descent_optimizer():
    alpha = 0.001
    w = torch.tensor([[5, 10.],[1, 2.]], requires_grad=True)
    optimizer = torch.optim.SGD([w],lr=alpha)

    for _ in range(500):
        func_res = (w + 7).log().log().prod()
        func_res.backward()
        # w.data -= alpha * w.grad
        optimizer.step()
        w.grad.zero_()
    print(w) 



if __name__ == '__main__':
    lol = 'frfrf'
    
    # gradient_descent_optimizer()
    print('Main')