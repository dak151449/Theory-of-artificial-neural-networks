import numpy as np

def grad(V, func, a, b):
    h = 1e-10  # Маленькое число для приближенного вычисления производной
    out = []
    for i in range(len(V)):
        W = V.copy()
        W[i] += h
        d1 = func(W)
        W[i] -= 2*h
        d2 = func(W)
        derivative = (d1 - d2) / (2 * h)  # Приближенное вычисление производной
        out.append(derivative)
    # derivative = (np.array(func(a, b, V + h)) - np.array(func(a, b, V - h))) / (2 * h)
    # print("grad в точке: ", V)
    return np.array(out)


def find_min(func, ak: float, bk: float, xk: list[float], dk: list[float], eps: float=1e-5) -> float:
    delta = eps / 2.0
    a = 0
    b = 5

    while abs(b - a) > eps:
        a1 = (a + b - delta) / 2.0
        b1 = (a + b + delta) / 2.0
        x1, x2 = [], []
        for i in range(len(xk)):
            x1.append(xk[i] + a1 * dk[i])
            x2.append(xk[i] + b1 * dk[i])
        if func(ak, bk, x1) < func(ak, bk, x2):
            b = b1
        else:
            a = a1
    return (a + b) / 2.0

def find_min2(func, ak: float, bk: float, xk: list[float], dk: list[float],a=-1, b=1, eps: float=1e-5) -> float:
    # a = 0
    # b = 10
    phi = (1 + 5**(1/2)) / 2
    x1 = b - (b - a) / phi
    x2 = a + (b - a) / phi


    while abs(b - a) > eps:
        if (func(xk + x1*dk) > func(xk + x2*dk)):
            a = x1
            x1 = b - (b - a) / phi
            x2 = a + (b - a) / phi
        else:
            b = x2
            x1 = b - (b - a) / phi
            x2 = a + (b - a) / phi
    return (a + b) / 2.0
