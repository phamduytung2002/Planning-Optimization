#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <bits/stdc++.h>
using namespace std;
#define N 100005
int n;          // number of queens
long long x[N]; // x[i] is the row of the queen on column i

long long row_nqueens[N], diag_nqueens[2 * N], adiag_nqueens[2 * N];
// number of queens on the i_th row, i_th diagonal, i_th anti-diagonal
// diagonal:        (N,1)                               // diag[1]
//                  (N-1, 1) - (N, 2)                   // diag[2]
//                  (N-2, 1) - (N-1, 2) - (N, 3)        // diag[3]
//                  ...
//                  (1,1) - (2,2) - (3,3) - ...         // diag[N]
//                  (1,2) - (2,3) - (3,4) - ...         // diag[N+1]
//                  ...
//                  (1,N-1) - (2,N)                     // diag[2*N-2]
//                  (1,N)                               // diag[2*N-1]
// anti-diagonal:   (1,1)                               // adiag[1]
//                  (1,2) - (2,1)                       // adiag[2]
//                  (1,3) - (2,2) - (3,1)               // adiag[3]
//                  ...
//                  (1,N) - (2,N-1) - ... - (N,1)       // adiag[N]
//                  (2,N) - (3,N-1) - ... - (N,2)       // adiag[N+1]
//                  ...
//                  (N-1,N) - (N,N-1)                   // adiag[2*N-2]
//                  (N,N)                               // adiag[2*N-1]

// (row, col) -> on diag[col-row+N]
//               on adiag[row+col-1]

long long n_violations;

long long truth_violations() {
    // TODO
    int v = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            if (x[i] == x[j])
                v++;
            if (x[i] + i == x[j] + j)
                v++;
            if (x[i] - i == x[j] - j)
                v++;
        }
    }
    return v;
}

long long violations() {
    // total number of violations
    int v = 0;
    for (int i = 1; i <= n; ++i)
        v += row_nqueens[i] * (row_nqueens[i] - 1) / 2;
    for (int i = 1; i <= 2 * n - 1; ++i)
        v += diag_nqueens[i] * (diag_nqueens[i] - 1) / 2;
    for (int i = 1; i <= 2 * n - 1; ++i)
        v += adiag_nqueens[i] * (adiag_nqueens[i] - 1) / 2;
    return v;
}

long long violationsQueen(int q) {
    // number of violations of a queen
    int v = 0;
    v += row_nqueens[x[q]] - 1;
    v += diag_nqueens[q - x[q] + n] - 1;
    v += adiag_nqueens[x[q] + q - 1] - 1;
    return v;
}

long long selectMostViolatingQueen() {
    int maxViolations = 0;
    int sel = -1;
    vector<int> cand;
    for (int q = 1; q <= n; q++) {
        int v = violationsQueen(q);
        if (v > maxViolations) {
            maxViolations = v;
            cand.clear();
            cand.push_back(q);
        } else if (v == maxViolations) {
            cand.push_back(q);
        }
    }
    int i = rand() % (cand.size()); // select a random index
    sel = cand[i];
    return sel;
}

long long getDelta(int q, int r) {
    // return the change of violations when queen q is moved to row r
    return -row_nqueens[x[q]] + row_nqueens[r] - diag_nqueens[q - x[q] + n] +
           diag_nqueens[q - r + n] - adiag_nqueens[x[q] + q - 1] +
           adiag_nqueens[r + q - 1] + 3;
}

pair<long long, long long> selectMostPromissingRow(int q) {
    long long minDelta = 1e9;
    long long sel_r = -1;
    vector<long long> cand;
    for (long long r = 1; r <= n; r++) {
        long long delta = getDelta(q, r);
        if (delta < minDelta) {
            minDelta = delta;
            cand.clear();
            cand.push_back(r);
        } else if (delta == minDelta) {
            cand.push_back(r);
        }
    }
    long long i = rand() % (cand.size());
    sel_r = cand[i];
    return {sel_r, minDelta};
}

void generateIntialSolution() {
    memset(row_nqueens, 0, sizeof(row_nqueens));
    memset(diag_nqueens, 0, sizeof(diag_nqueens));
    memset(adiag_nqueens, 0, sizeof(adiag_nqueens));
    for (int i = 1; i <= n; i++)
        x[i] = 1;
    row_nqueens[1] = n;
    for (int i = n; i <= 2 * n - 1; ++i)
        diag_nqueens[i] = 1;
    for (int i = 1; i <= n; ++i)
        adiag_nqueens[i] = 1;
    n_violations = violations();
}

void searchforever() {
    generateIntialSolution();
    int it = 0;
    while (true) {
        it++;
        int q = selectMostViolatingQueen();
        pair<int, int> r_delta = selectMostPromissingRow(q);
        int r = r_delta.first;
        int delta = r_delta.second;
        n_violations += delta;
        row_nqueens[x[q]]--;
        diag_nqueens[q - x[q] + n]--;
        adiag_nqueens[x[q] + q - 1]--;
        x[q] = r;
        row_nqueens[x[q]]++;
        diag_nqueens[q - x[q] + n]++;
        adiag_nqueens[x[q] + q - 1]++;
        // if (it % 100 == 0)
        //     cout << "Step " << it << ": violations = " << n_violations
        //          << " move(" << q << "," << r << ")" << endl;
        // if (n_violations != truth_violations()) {
        //     cout << "wrong violations";
        //     return;
        // }
        if (n_violations == 0) {
            cout << n << endl;
            for (int i = 1; i <= n; ++i)
                cout << x[i] << " ";
            // cout << truth_violations() << endl;
            return;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    srand(time(NULL));
    cin >> n;
    searchforever();
    return 0;
}
