import numpy as np
from itertools import permutations
import time
import random

# from numba.experimental import jitclass
# from numba import njit, int64


n = int(input())
distance_matrix = np.array(
    [list(map(int, input().split())) for _ in range(n)], dtype=np.int64
)


class Solution:
    def __init__(self, n, distance_matrix):
        self.n = n
        self.distance_matrix = distance_matrix
        self.path = np.arange(n)
        self.cost = self.calcuate_cost()

    def calcuate_cost(self):
        cost = 0
        for i in range(self.n):
            cost += self.distance_matrix[self.path[i - 1], self.path[i]]
        return cost


def twoOpt_func(sol: Solution, i, j):
    new_sol = Solution(sol.n, sol.distance_matrix)
    new_sol.path = np.copy(sol.path)
    new_sol.path[i:j] = sol.path[i:j][::-1]
    new_sol.cost = new_sol.calcuate_cost()
    return new_sol


def twoOpt(sol: Solution):
    best_sol = sol
    for i in range(n):
        for j in range(i + 1, n):
            new_sol = twoOpt_func(sol, i, j)
            if new_sol.cost < best_sol.cost:
                best_sol = new_sol
    return best_sol


def swap2_func(sol: Solution, i, j) -> Solution:
    new_sol = Solution(sol.n, sol.distance_matrix)
    new_sol.path = np.copy(sol.path)
    new_sol.path[i], new_sol.path[j] = sol.path[j], sol.path[i]
    new_sol.cost = (
        sol.cost
        + sol.distance_matrix[sol.path[i - 1], sol.path[j]]
        + sol.distance_matrix[sol.path[j], sol.path[(i + 1) % sol.n]]
        + sol.distance_matrix[sol.path[j - 1], sol.path[i]]
        + sol.distance_matrix[sol.path[i], sol.path[(j + 1) % sol.n]]
        - sol.distance_matrix[sol.path[i - 1], sol.path[i]]
        - sol.distance_matrix[sol.path[i], sol.path[(i + 1) % sol.n]]
        - sol.distance_matrix[sol.path[j - 1], sol.path[j]]
        - sol.distance_matrix[sol.path[j], sol.path[(j + 1) % sol.n]]
    )
    return new_sol


def swap2(sol: Solution):
    best_sol = sol
    for i in range(n):
        for j in range(i + 1, n):
            new_sol = swap2_func(sol, i, j)
            if new_sol.cost < best_sol.cost:
                best_sol = new_sol
    return best_sol


init_solution = Solution(n, distance_matrix)
best_solution = init_solution

begin_time = time.time()
time_limit = 3
iter = 0
patient = 5
waited = 0
while time.time() - begin_time < time_limit - 1:
    iter += 1
    if random.random() < 0.5:
        new_solution = swap2(best_solution)
    else:
        new_solution = twoOpt(best_solution)

    if new_solution.cost < best_solution.cost:
        best_solution = new_solution
        waited = 0
    else:
        waited += 1
        if waited >= patient:
            break

best_solution.path += 1
print(n)
for i in best_solution.path:
    print(i, end=" ")
