##### CP

from ortools.sat.python import cp_model
import numpy as np
import os
os.environ['OPENBLAS_NUM_THREADS'] = '6'

model = cp_model.CpModel()
solver = cp_model.CpSolver()

M = 1000

n, k = map(int, input().split())
c = np.zeros((2*n+1, 2*n+1), np.int_)

for i in range(2*n+1):
    c[i] = list(map(int, input().split()))

x = {}  # x[i, j] = 1 if the bus go from i to j
u = {}  # u[i] = order of visiting i
v = {} #  v[i] = number of passengers after stop at i

sum_row = {}
sum_col = {}

for i in range(2*n+1):
    u[i] = model.NewIntVar(0, 2*n, f"u[{i}]")
    v[i] = model.NewIntVar(0, k, f"v[{i}]")
    sum_row[i] = 0
    sum_col[i] = 0
    for j in range(2*n+1):
        x[i, j] = model.NewBoolVar(f"x[{i},{j}]")

for i in range(2*n+1):
    for j in range(2*n+1):  
        sum_row[i] += x[i, j]
        sum_col[j] += x[i, j]

model.Add(u[0] == 0)
model.Add(v[0] == 0)

for i in range(2*n+1):
    model.Add(sum_row[i] == 1)
    model.Add(sum_col[i] == 1)
    model.Add(x[i, i] != 1)

for i in range(2*n+1):
    for j in range(2*n+1):
        if i != j and j != 0:
            # CP: model.Add(u[i]+1-u[j] >= M*(x[i,j]-1)).OnlyEnforceIf(x[i, j])
            model.Add(u[j]-1-u[i] >= M*(x[i,j]-1))
            # model.Add(u[i]+1<=u[j]).OnlyEnforceIf(x[i, j])

for i in range(1, n+1):
    model.Add(u[i] < u[i+n])

for j in range(1, n):
    for i in range(0, 2*n+1):
        model.Add(v[j]-v[i]-1>=M*(x[i, j]-1))
        model.Add(v[i]+1-v[j]>=M*(x[i, j]-1))
        # model.Add(v[j]==v[i]+1).OnlyEnforceIf(x[i, j])

for j in range(n+1, 2*n+1):
    for i in range(0, 2*n+1):
        model.Add(v[j]-v[i]+1>=M*(x[i, j]-1))
        model.Add(v[i]-1-v[j]>=M*(x[i, j]-1))
        # model.Add(v[j]==v[i]-1).OnlyEnforceIf(x[i, j])

cost = 0
for i in range(2*n+1):
    for j in range(2*n+1):
        cost += c[i, j]*x[i, j]

model.Minimize(cost)

status = solver.Solve(model,)
# print(status)
if status == cp_model.OPTIMAL:
    print(solver.Value(cost))
    # print('route: ')
    # for i in range(2*n+1):
    #     for j in range(2*n+1):
    #         if solver.Value(x[i, j]) == 1:
    #             print(i, j)

    # print('u: ')
    # for i in range(2*n+1):
    #     print(i, solver.Value(u[i]))

    # print('v: ')
    # for i in range(2*n+1):
    #     print(i, solver.Value(v[i]))


exit()

##### IP

from ortools.sat.python import cp_model
import numpy as np

model = cp_model.CpModel()
solver = cp_model.CpSolver()

n, k = map(int, input().split())
c = np.zeros((2*n+1, 2*n+1), np.int_)

for i in range(2*n+1):
    c[i] = list(map(int, input().split()))

x = {}  # x[i, j] = 1 if the bus go from i to j
u = {}  # u[i] = order of visiting i
v = {} #  v[i] = number of passengers after stop at i

sum_row = {}
sum_col = {}

for i in range(2*n+1):
    u[i] = model.NewIntVar(0, 2*n, f"u[{i}]")
    v[i] = model.NewIntVar(0, k, f"v[{i}]")
    sum_row[i] = 0
    sum_col[i] = 0
    for j in range(2*n+1):
        x[i, j] = model.NewBoolVar(f"x[{i},{j}]")

for i in range(2*n+1):
    for j in range(2*n+1):  
        sum_row[i] += x[i, j]
        sum_col[j] += x[i, j]

model.Add(u[0] == 0)
model.Add(v[0] == 0)

for i in range(2*n+1):
    model.Add(sum_row[i] == 1)
    model.Add(sum_col[i] == 1)
    model.Add(x[i, i] != 1)

for i in range(2*n+1):
    for j in range(2*n+1):
        if i != j and j != 0:
            model.Add(u[i]+1==u[j]).OnlyEnforceIf(x[i, j])
            model.Add(u[i]+1!=u[j]).OnlyEnforceIf(x[i, j].Not())

for i in range(1, n+1):
    model.Add(u[i] < u[i+n])

for i in range(0, 2*n+1):
    for j in range(1, 2*n+1):
        model.Add(v[j]==v[i]+1).OnlyEnforceIf(j<=n and x[i, j])
        model.Add(v[j]==v[i]-1).OnlyEnforceIf(j>n and x[i, j])

cost = 0
for i in range(2*n+1):
    for j in range(2*n+1):
        cost += c[i, j]*x[i, j]

model.Minimize(cost)

status = solver.Solve(model,)
# print(status)
if status == cp_model.OPTIMAL:
    print(solver.Value(cost))
    # print('route: ')
    # for i in range(2*n+1):
    #     for j in range(2*n+1):
    #         if solver.Value(x[i, j]) == 1:
    #             print(i, j)

    # print('u: ')
    # for i in range(2*n+1):
    #     print(i, solver.Value(u[i]))

    # print('v: ')
    # for i in range(2*n+1):
    #     print(i, solver.Value(v[i]))