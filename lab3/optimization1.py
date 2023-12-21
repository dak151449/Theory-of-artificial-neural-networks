from cmath import sqrt
from aux import *
import numpy as np
import matplotlib.pyplot as plt

def getMinArg(f, Xk, Dk, a, b):
    step = 1e-6
    a, b = min(a, b), max(a, b) 
    alpha = -1
    minalpha = a
    nXk = Xk
    nDk = Dk
    minF = f(nXk + alpha*nDk)
    while alpha < 1:
        if f(nXk + alpha*nDk) < minF:
            minF = f(nXk + alpha*nDk)
            minalpha = alpha
        alpha += step
    return minalpha

def norm(V):
    return max(map(abs, V))

def get_hessian(x, a, b):
    n = len(x)
    hessian = np.zeros((len(x), len(x)))
    hessian[0][0] = 12 * a * x[0] ** 2 - 4 * a * x[1] + 2 * b
    hessian[0][1] = -4 * a * x[0]

    for i in range(1, n - 1):
        hessian[i][i - 1] = -4 * a * x[i - 1]
        hessian[i][i] = 12 * a * x[i] ** 2 - 4 * a * x[i + 1] + 2 * b + 2 * a
        hessian[i][i + 1] = -4 * a * x[i]

    hessian[n - 1][n - 2] = -4 * a * x[n - 2]
    hessian[n - 1][n - 1] = 2 * a

    return hessian

# метод наискорейшего спуска
def opt1(x0, func, func_grad, a, b):
    Xs = []
    Ys = []
    k = 0
    Xk = np.array(x0)
    Xk_back = Xk
    M = 10**4

    dk = -grad(Xk, func, a, b)

    eps1 = 1e-2
    eps2 = 1e-1
    index = 0
    iter = 0
    while index < M and iter < 100:
        # gradF = grad(Xk, func, a, b)

        # gradF = func_grad(Xk)
        gradF = -grad(Xk, func, a, b)
        if  np.linalg.norm(gradF) < eps1:
            plt.plot(Xs[3:], Ys[3:])
            plt.title("Флетчера-Ривз")
            plt.savefig("f_r.png")
            plt.show()
            return Xk, index
        #step 6
        alpha2 = find_min2(func, a, b, Xk, dk, a=-1, b=1)
        alpha = alpha2
        
        X_new = Xk + alpha*dk
        wk = (norm(grad(X_new, func, a, b)))**2 / (norm(grad(Xk, func, a, b)))**2
        # wk = 0
        # dk = - grad(Xk, func, a, b) + wk*dk
        dk = -grad(X_new, func, a, b) + wk*dk
       

        # step 8
        # print(X_new - Xk)
        if (norm(X_new - Xk) < eps1 and abs(func(X_new) - func(Xk)) < eps2) and \
            (norm(Xk - Xk_back) < eps1 and abs(func(Xk) - func(Xk_back)) < eps2):
            print("Answer: ", X_new)
            plt.plot(Xs[3:], Ys[3:])
            plt.title("Флетчера-Ривз")
            plt.show()
            plt.savefig("f_r.png")
            return X_new, index
        
        # Xk_back = Xk.copy()
        Xs.append(iter)
        Ys.append(func(X_new))
        Xk = X_new.copy()
        # print(index, Xk, dk, alpha1, alpha2)
        index += 1
        iter += 1
    
    plt.plot(Xs[3:], Ys[3:])
    plt.title("Флетчера-Ривз")
    plt.savefig("f_r.png")
    plt.show()
    return Xk, index
        
# метод наискорейшего спуска
def polak_r(x0, func, func_grad, a, b):
    Xs = []
    Ys = []
    k = 0
    Xk = np.array(x0)
    Xk_back = Xk
    M = 10**4

    dk = -grad(Xk, func, a, b)

    eps1 = 1e-2
    eps2 = 1e-1
    index = 0
    iter = 0
    while index < M :
        gradF = -grad(Xk, func, a, b)
        if  np.linalg.norm(gradF) < eps1:
            plt.plot(Xs[3:], Ys[3:])
            plt.title("Полака-Рибьер")
            plt.savefig("polak_r.png")
            # plt.show()
            return Xk, index
        #step 6
        alpha2 = find_min2(func, a, b, Xk, dk, a=0, b=1)
        alpha = alpha2
        
        X_new = Xk + alpha*dk
        # wk = (norm(grad(X_new, func, a, b)))**2 / (norm(grad(Xk, func, a, b)))**2
        wk = grad(X_new, func, a, b) * (grad(X_new, func, a, b) - grad(Xk, func, a, b))
        wk = wk / (norm(grad(Xk, func, a, b)))**2
        # wk = 0
        # dk = - grad(Xk, func, a, b) + wk*dk
        dk = -grad(X_new, func, a, b) + wk*dk
       

        # step 8
        # print(X_new - Xk)
        if (norm(X_new - Xk) < eps1 and abs(func(X_new) - func(Xk)) < eps2) and \
            (norm(Xk - Xk_back) < eps1 and abs(func(Xk) - func(Xk_back)) < eps2):
            print("Answer: ", X_new)
            plt.plot(Xs[3:], Ys[3:])
            plt.title("Полака-Рибьер")
            plt.savefig("polak_r.png")
            # plt.show()
            return X_new, index
        
        # Xk_back = Xk.copy()
        Xs.append(index)
        Ys.append(func(X_new))
        Xk = X_new.copy()
        # print(index, Xk, dk, alpha1, alpha2)
        index += 1
        iter += 1

    plt.plot(Xs[3:], Ys[3:])
    plt.title("Полака-Рибьер")
    plt.savefig("polak_r.png")
    # plt.show()
    
    return Xk, index
        
def dev_flet_paul(x0, func, func_grad, a, b):
    Xs = []
    Ys = []
    Xk = np.array(x0)
    Xk_back = Xk.copy()
    G = np.eye(len(x0))
    M = 10**6

    dk = -grad(Xk, func, a, b)

    eps1 = 1e-12
    eps2 = 1e-12
    index = 0
    iter = 0
    while index < M:
        # gradF = grad(Xk, func, a, b)

        # gradF = func_grad(Xk)
        gradF = -grad(Xk, func, a, b)
        if  np.linalg.norm(gradF) < eps1:
            plt.plot(Xs[3:], Ys[3:])
            plt.title("Девидона-Флетчера-Пауэл")
            plt.savefig("dev_flet_paul.png")
            plt.show()
            return Xk, index

        if (iter != 0):
            g = grad(Xk, func, a, b) - grad(Xk_back, func, a, b)
            dx = Xk - Xk_back

            dG = ((dx @ dx.T) / (dx.T @ g)) - ((G @ g @ g.T * G.T) / (g.T @ G @ g))
            G = G + dG
        else:
            G = np.eye(len(x0))
        dk = -G @ grad(Xk, func, a, b)
        # print(G, '\n',dk, Xk, np.dot(G,grad(Xk, func, a, b)))
        alpha2 = find_min2(func, a, b, Xk, dk, a=0, b=0.5)
        
        X_new = Xk + alpha2*dk
        # step 8
        print(X_new, Xk)
        if (norm(X_new - Xk) < eps1 and abs(func(X_new) - func(Xk)) < eps2):
            print("Answer: ", X_new)
            plt.plot(Xs[3:], Ys[3:])
            plt.title("Девидона-Флетчера-Пауэл")
            plt.savefig("dev_flet_paul.png")
            plt.show()
            return X_new, index
        
        Xs.append(index)
        Ys.append(func(X_new))
        Xk_back = Xk.copy()
        Xk = X_new.copy()

        index += 1
        iter += 1

    plt.plot(Xs[3:], Ys[3:])
    plt.title("Девидона-Флетчера-Пауэл")
    plt.savefig("dev_flet_paul.png")
    plt.show()
    return Xk, index    

def lev_mar(x0, func, func_grad, a, b):
    Xs = []
    Ys = []
    Xk = np.array(x0)
    M = 10**6

    dk = -grad(Xk, func, a, b)

    mu = 10**4
    eps1 = 1e-4
    index = 0
    iter = 0
    while index < M:
        gradF = -grad(Xk, func, a, b)
        H_new = np.eye(len(x0))
        if  np.linalg.norm(gradF) < eps1:
            plt.plot(Xs[3:], Ys[3:])
            plt.title("Левенберг-Марквардт")
            plt.show()
            plt.savefig("lev_mar.png")
            return Xk, index

        if (iter != 0):
            H_new = get_hessian(Xk, a, b)
        
        dk = np.linalg.inv(H_new + mu*np.eye(len(x0))) @ grad(Xk, func, a, b)
        X_new = Xk - dk
        if (func(X_new) < func(Xk)):
            mu = mu/2
        else:
            mu = 2*mu
            continue
        Xs.append(index)
        Ys.append(func(X_new))
        Xk = X_new.copy()
        index += 1
        iter += 1
    plt.plot(Xs[3:], Ys[3:])
    plt.title("Левенберг-Марквардт")
    plt.show()
    plt.savefig("lev_mar.png")
    return Xk, index  