import random
import math
from scipy.optimize import fmin
import copy
import matplotlib.pyplot as plt


def GEN_grad1(n, task_a, task_b, task_fn):
    Mp = 100
    Np = 1000
    pop = [[random.uniform(-10.0, 10.0) for _ in range(0, n)] for _ in range(0, Mp)]
    xs = []
    ys = []
    for t in range(0, Np):
        xs.append(t)
        fitness = [1 / task_fn(pop[i]) for i in range(0, Mp)]
        fit = 0.0
        for i in range(0, Mp):
            fit += fitness[i]
        prob = [0.0 for i in range(0, Mp)]
        for i in range(0, Mp):
            for j in range(0, i + 1):
                prob[i] += fitness[j] / fit
        prob = [0.0] + prob
        crossd = []
        for i in range(0, Mp):
            r = random.uniform(0.0000001, 1.0)
            for j in range(1, Mp + 1):
                if prob[j - 1] < r and prob[j] >= r:
                    crossd.append([pop[j - 1]])
        for i in range(0, Mp):
            esc = False
            while True:
                r = random.uniform(0.0000001, 1.0)
                for j in range(1, Mp + 1):
                    if prob[j - 1] < r and prob[j] >= r and crossd[i][0] != pop[j - 1]:
                        crossd[i].append(pop[j - 1])
                        esc = True
                if esc:
                    break
        n_pop = []
        for i in range(0, Mp):
            c = random.uniform(0.0000001, 0.9999999)
            n_x = [c * crossd[i][0][j] + (1 - c) * crossd[i][1][j] for j in range(0, n)]
            n_pop.append(n_x)
        print("new population:", n_pop)
        Pm = random.uniform(0.05, 0.2)
        mut = []
        for i in range(0, Mp):
            r = random.uniform(0.0, 1.0)
            if r < Pm:
                mut.append(n_pop[i])
        for i in range(0, len(mut)):
            ii = random.randint(0, n - 1)
            mut[i][ii] = random.uniform(-10.0, 10.0)
        if len(mut) > 0:
            fitness = [1 / task_fn(n_pop[i]) for i in range(0, Mp)]
            ii = random.randint(0, len(mut) - 1)
            mn = 99999999
            mn_i = 0
            for j in range(0, Mp):
                if fitness[j] < mn:
                    mn = fitness[j]
                    mn_i = j
            print(n_pop[mn_i], "mutated into", mut[ii])
            n_pop[mn_i] = mut[ii]
        for i in range(0, Mp):
            pop[i] = n_pop[i]
        fitness = [1 / task_fn(pop[i]) for i in range(0, Mp)]
        mx = 0.0
        mx_i = 0
        for i in range(0, Mp):
            if fitness[i] > mx:
                mx = fitness[i]
                mx_i = i
        ys.append(1 / mx)
    fitness = [1 / task_fn(pop[i]) for i in range(0, Mp)]
    mx = 0.0
    mx_i = 0
    for i in range(0, Mp):
        if fitness[i] > mx:
            mx = fitness[i]
            mx_i = i
    print(pop[mx_i])

    plt.plot(xs[:5], ys[:5])
    plt.title("Генетический алгоритм")
    plt.savefig("gen.png")
    plt.show()

