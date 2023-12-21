
from optimization1 import *
from gen_alg import *
f0 = 15
a = 100
b = 15

def rozenbrok(x, a=1000, b=200):
    s = 0
    for i in range(len(x) - 1):
        s += a*(x[i]**2 - x[i+1])**2 + b*(x[i] - 1)**2

    s += b*(x[-1] - 1)**2
    return s + f0

def rozenbrok_grad(x):
    df_dx0 = 4 * a * x[0] * (x[0]**2 - x[1]) + 2 * b * (x[0] - 1)
    n = len(x)
    df_dx1 = -2 * a * (x[n-2]**2 - x[n - 1])
    return np.array([df_dx0, df_dx1])

def main():
    x = np.array([0.3, 2])
    a = 100
    b = 15

    # print(rozenbrok([-2, 2]))
    
    answ, count = opt1(x, rozenbrok, rozenbrok_grad, a, b)
    print("Флетчера-Ривз: ", answ, rozenbrok(answ), count)
    answ, count = polak_r(x, rozenbrok, rozenbrok_grad, a, b)
    print("Полака-Рибьер: ", answ, rozenbrok(answ), count)
    answ, count = dev_flet_paul(x, rozenbrok, rozenbrok_grad, a, b)
    print("Девидона-Флетчера-Пауэл: ", answ, rozenbrok(answ), count)
    
    answ, count = lev_mar(x, rozenbrok, rozenbrok_grad, a, b)
    print("Левенберг-Марквардт: ", answ, rozenbrok(answ), count)

    GEN_grad1(len(x), a, b, rozenbrok)
    # plt.plot(xs, ys)
    # plt.title("Генетический алгоритм")
    # plt.show()
    return

main()