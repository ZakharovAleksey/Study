import os
import torch
import numpy as np

# Бинарная кросс энтропия (сигмиоидная функция активации)
# BCE(p, t) = -t log(p) - (1-t) log(1 - p)
# Можно использовать с сигмиоидой на последнем слое, и при задачах класфикации

# Многоклассовая классификация: несколько выходов нейронной сети
# каждый выход - вероятность того что у нас именно этот объект
# на картинке. То есть считаем число на каждый выход, а потом применяем
# софтмакс (см. далее).
# Активация: софтмакс
# SM_{i}(y) = e^{yi}/\sum_{i}e^{y_{i}};
# Свойства:
# 1. 0 < SM < 1
# 2. sum_{i}(SM_{i}) = 1, сумма по всем классам равна 1. (вероятность)
#
# Потери: Кросс энтропия CE(p, t) = -\sum_{j}t_{j}*log(p_{j}),
# p - вероятность SM

# Локализация
'''
    Output number: 5
                        -> Probability cat
    [picture] -> [NN]   -> <x, y> - center of cat
                        -> <h, w> - size of area with cat

    Activation: 
    1. Probability: Sigmioid in [0, 1]
    2. <x,y> - Sigmioid in (0, 1) -> 0.5, 0.5 - middle of the picture
    3. <h, w> - exp(corresponding output of NN)

    Loss function: 
    1. Probability: Binary cross entropy: BCE(sigma(y), )
    2. <x, y>: BCE(sigma(y), )
    3. <h, w>: MSE(y, log(wight/height))


'''

# Сегментация
'''
    

'''
if __name__ == '__main__':
    print(np.zeros(10))