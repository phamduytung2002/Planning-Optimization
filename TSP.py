from ortools.linear_solver import pywraplp
import numpy as np
from itertools import chain, combinations


def powerset(iterable):
    "powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)"
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(len(s) + 1))


# input
n = int(input())
c = np.zeros((n, n))
for i in range(n):
    c[i] = list(map(int, input().split()))

# solver
solver = pywraplp.Solver("TSP", pywraplp.Solver.CBC_MIXED_INTEGER_PROGRAMMING)
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
# subtour elimination
for subset in powerset(range(n)):
    if len(subset) > 1 and len(subset) < n:
        solver.Add(
            solver.Sum([x[i, j] for i in subset for j in subset]) <= len(subset) - 1
        )


# objective
cost = 0
for i in range(n):
    for j in range(n):
        cost += c[i, j] * x[i, j]
solver.Minimize(cost)

# solve
status = solver.Solve()
if status == pywraplp.Solver.OPTIMAL:
    # print('Objective value =', solver.Objective().Value())
    print(n)
    answer = [1]
    i = 0
    while True:
        for j in range(n):
            if x[i, j].solution_value() == 1:
                answer.append(j + 1)
                i = j
                break
        if j == 0:
            answer.pop()
            break
    print(*answer)
