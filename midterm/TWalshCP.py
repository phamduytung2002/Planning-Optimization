from ortools.sat.python import cp_model

model = cp_model.CpModel()

A = model.NewIntVar(1, 8, "A")
B = model.NewIntVar(1, 8, "B")
C = model.NewIntVar(1, 8, "C")
D = model.NewIntVar(1, 8, "D")
E = model.NewIntVar(1, 8, "E")
F = model.NewIntVar(1, 8, "F")
G = model.NewIntVar(1, 8, "G")
H = model.NewIntVar(1, 8, "H")

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

for edge in edges:
    model.Add(edge[0] - edge[1] !=1)
    model.Add(edge[0] - edge[1] !=-1)

model.AddAllDifferent(vertices)
model.Add(G < H)
model.Add(B < F)

solver = cp_model.CpSolver()
status = solver.Solve(model)


if status == cp_model.FEASIBLE or status == cp_model.OPTIMAL:
    for vertex in vertices:
        print(solver.Value(vertex), end=" ")
else:
    print(status)