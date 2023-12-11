#include <bits/stdc++.h>
using namespace std;

const int maxn = 1002, maxm = 1002;
double c[maxn], A[maxm + 1][2 * maxn], b[maxm], E[maxm + 1], x[maxn];

// n+m+1 col: z
// RHS: b

/*
Description
Solve the LP problem with Simplex method under the Tabular form

         max f(x) = C[1]*X[1] + C[2]*X[2] +  . . . + C[n]*X[n]
s.t.
    A[1,1]*X[1] + A[1,2]*X[2] + . . . + A[1,n]*X[n]   <= b[1]
    A[2,1]*X[1] + A[2,2]*X[2] + . . . + A[2,n]*X[n]   <= b[2]
             . . .
    A[m,1]*X[1] + A[m,2]*X[2] + . . . + A[m,n]*X[n]   <= b[m]
domain of variables:   0 <= X[i], for i = 1, 2, ..., n

Input
Line 1: contains 2 positive integer n and m (1 <= n,m <= 1000)
Line n+1: contains C[1], C[2], ..., C[n]
Line n+2+i (i = 1,...,m): contains the ith row of the matrix A: A[i,1], A[i,2],
. . ., A[i,n] Line n+m+2: contains b[1], b[2], ..., b[m]

Output
If the given problem does not have any optimal solution, then write UNBOUNDED.
Otherwise, line 1 writes n, and line 2 writes X[1], X[2], ..., X[n] (separated
by a SPACE character)
*/

int n, m;

void input() {
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        cin >> c[i];
    }
    for (int j = 1; j <= m; ++j) {
        for (int i = 1; i <= n; ++i) {
            cin >> A[j][i];
        }
    }
    for (int j = 1; j <= m; ++j) {
        cin >> b[j];
    }
    for (int i = 1; i <= m + 1; ++i) {
        A[i][n + i] = 1;
    }
    for (int i = 1; i <= n; ++i) {
        A[m + 1][i] = -c[i];
    }
}

int find_colpivot() {
    int min_neg = 0, min_neg_id = 0;
    for (int i = 1; i <= n + m; ++i) {
        if (A[m + 1][i] < min_neg) {
            min_neg = A[m + 1][i];
            min_neg_id = i;
        }
    }
    return min_neg_id;
}

void calculate_E(int colpivot) {
    for (int i = 1; i <= m; ++i) {
        if (A[i][colpivot] > 0) {
            E[i] = b[i] / A[i][colpivot];
        } else {
            E[i] = INT_MAX;
        }
    }
}

int find_rowpivot() {
    int min_val = INT_MAX, min_id = 0;
    for (int i = 1; i <= m; ++i) {
        if (E[i] < min_val) {
            min_val = E[i];
            min_id = i;
        }
    }
    return min_id;
}

void update_row(int rowpivot, int colpivot) {
    double pivot = A[rowpivot][colpivot];
    for (int i = 1; i <= n + m + 1; ++i) {
        A[rowpivot][i] /= pivot;
    }
    b[rowpivot] /= pivot;
    for (int row = 1; row <= m + 1; row++) {
        if (row == rowpivot)
            continue;
        double multiplier = A[row][colpivot];
        for (int col = 1; col <= m + n + 1; col++) {
            A[row][col] -= A[rowpivot][col] * multiplier;
        }
        b[row] -= b[rowpivot] * multiplier;
    }
}

bool check_unbounded(int col) {
    for (int i = 1; i <= m; i++) {
        if (A[i][col] > 0)
            return false;
    }
    return true;
}

void print_matrix() {
    for (int i = 1; i <= m + 1; i++) {
        for (int j = 1; j <= n + m + 1; j++) {
            cout << A[i][j] << " ";
        }
        cout << b[i];
        cout << endl;
    }
}

void get_answer() {
    for (int i = 1; i <= n; ++i) {
        for (int col = 1; col <= m; ++col) {
            if (A[col][i] == 1) {
                x[i] = b[col];
                break;
            }
        }
    }
}

int main() {
    input();
    while (1) {
        int colpivot = find_colpivot();
        if (colpivot == 0) {
            get_answer();
            cout << n << endl;
            for (int i = 1; i <= n; ++i) {
                cout << fixed << setprecision(1) << x[i] << " ";
            }
            return 0;
        }
        calculate_E(colpivot);
        int rowpivot = find_rowpivot();
        if (check_unbounded(colpivot)) {
            cout << "UNBOUNDED" << endl;
            return 0;
        }
        update_row(rowpivot, colpivot);
    }
}