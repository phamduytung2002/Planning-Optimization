from ortools.linear_solver import pywraplp

solver = pywraplp.Solver.CreateSolver("SAT")

M = 1000
eps = 0.5

A = solver.IntVar(1, 8, "A")
B = solver.IntVar(1, 8, "B")
C = solver.IntVar(1, 8, "C")
D = solver.IntVar(1, 8, "D")
E = solver.IntVar(1, 8, "E")
F = solver.IntVar(1, 8, "F")
G = solver.IntVar(1, 8, "G")
H = solver.IntVar(1, 8, "H")

vertices = [A, B, C, D, E, F, G, H]
edges = [
    (A, B),
    (A, G),
    (A, F),
    (B, C),
    (B, G),
    (B, H),
    (G, H),
    (G, C),
    (G, E),
    (G, F),
    (F, E),
    (F, H),
    (C, H),
    (C, D),
    (H, E),
    (H, D),
    (E, D),
]

z = {}
for i in range(len(edges)):
    z[i*2] = solver.BoolVar("z" + str(i)+"0")
    solver.Add(edges[i][0] - edges[i][1] - 1 >= eps - M * z[i*2])
    solver.Add(-edges[i][0] + edges[i][1] + 1 >= eps - M * (1 - z[i*2]))

    z[i*2+1] = solver.BoolVar("z" + str(i)+"1")
    solver.Add(edges[i][0] - edges[i][1] + 1 >= eps - M * z[i*2+1])
    solver.Add(-edges[i][0] + edges[i][1] - 1 >= eps - M * (1 - z[i*2+1]))

t ={}
for i in range(len(vertices)):
    for j in range(i+1, len(vertices)):
        t[i, j] = solver.BoolVar("t" + str(i) + str(j))
        solver.Add(vertices[i] - vertices[j] >= eps - M * t[i, j])
        solver.Add(-vertices[i] + vertices[j] >= eps - M * (1 - t[i, j]))

solver.Add(G <= H - 1)
solver.Add(B <= F - 1)

status = solver.Solve()


if status == pywraplp.Solver.FEASIBLE or status == pywraplp.Solver.OPTIMAL:
    for vertex in vertices:
        print(int(vertex.solution_value()), end=" ")
else:
    print()