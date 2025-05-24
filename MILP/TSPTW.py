from ortools.linear_solver import pywraplp
import numpy as np
from itertools import chain, combinations

# 0-based index

M = 1e5


def powerset(iterable):
    "powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)"
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(len(s) + 1))


# input n
n = int(input())
n += 1

# input l, e, d
l = [0]
e = [0]
d = [0]
for i in range(1, n):
    li, ei, di = map(int, input().split())
    l.append(li)
    e.append(ei)
    d.append(di)

# input t
t = np.zeros((n, n))
for i in range(n):
    t[i] = list(map(int, input().split()))

# solver
solver = pywraplp.Solver("TSP", pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)


# objective
cost = 0

# decision variable: x[i, j] if the path goes from i to j
x = {}
for i in range(n):
    for j in range(n):
        x[i, j] = solver.BoolVar("x[%i,%i]" % (i, j))

# constraint
# 1 out
for i in range(n):
    solver.Add(solver.Sum([x[i, j] for j in range(n)]) == 1)
# 1 in
for j in range(n):
    solver.Add(solver.Sum([x[i, j] for i in range(n)]) == 1)
# directed
for i in range(n):
    for j in range(n):
        solver.Add(x[i, j] + x[j, i] <= 1)
# no loop
for i in range(n):
    solver.Add(x[i, i] == 0)

# # subtour elimination 1
# for subset in powerset(range(n)):
#     if len(subset) > 1 and len(subset) < n:
#         solver.Add(
#             solver.Sum([x[i, j] for i in subset for j in subset]) <= len(subset) - 1
#        )

# subtour elimination 2 - MTZ method
u = {}
for i in range(n):
    u[i] = solver.IntVar(0, n-1, f"u[{i}]")
for i in range(n):
    for j in range(1, n):
        solver.Add(u[i] - u[j] + n * x[i, j] <= n - 1)

# arrival time
a = {}  # time visit each node
z = {}  # aux variable for the constrain: x[i,j] = 0 or a[i] + t[i,j] - a[j] = 0
a[0] = 0
for i in range(1, n):
    a[i] = solver.IntVar(0, M, f"a[{i}]")
for i in range(n):
    for j in range(1, n):
        z[i, j] = solver.BoolVar(f"z[{i},{j}]")
        solver.Add(M * z[i, j] >= a[i] + t[i, j] + d[i] - a[j])
        solver.Add(M * z[i, j] >= -a[i] - t[i, j] - d[i] + a[j])
        solver.Add(z[i, j] + x[i, j] <= 1)
        cost += z[i,j]

# # TW constraint
# for i in range(n):
#     solver.Add(a[i] <= l[i])
#     solver.Add(a[i] >= e[i])

for i in range(n):
    for j in range(n):
        cost += t[i, j] * x[i, j]
solver.Minimize(cost)

# solve
status = solver.Solve()
# print('feasible code: ', pywraplp.Solver.FEASIBLE)
# print('infeasible code: ', pywraplp.Solver.INFEASIBLE)
# print(status)
if status == pywraplp.Solver.OPTIMAL:
    # print('Objective value =', solver.Objective().Value())
    print(n - 1)
    answer = []
    i = 0
    while True:
        for j in range(n):
            if x[i, j].solution_value() == 1:
                answer.append(j)
                i = j
                break
        if j == 0:
            answer.pop()
            break
    print(*answer)

    # for i in range(n):
    #     print(u[i].solution_value())

    # for i in range(n):
    #     if i != 0:
    #         print(a[i].solution_value())
    #     else:
    #         print(a[i])
    # for i in range(n):
    #     for j in range(1, n):
    #         print(f'z[{i}, {j}]: {z[i, j].solution_value()}')
    # for i in range(1, n):
    #     for j in range(n):
    #         print(f'x[{i}, {j}]: {x[i, j].solution_value()}')