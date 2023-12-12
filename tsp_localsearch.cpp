#define NDEBUG
#include <bits/stdc++.h>
using namespace std;

const int VERY_BIG_NUMBER = 1e8;
const double MAXTIME = 110;
const int MAXITER = 1000;
const int maxn = 105;

class Problem {
  public:
    int n, c[maxn][maxn];

    Problem() {}

    void input() {
        cin >> this->n;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> this->c[i][j];
            }
            this->c[i][n] = c[i][0];
            this->c[n][i] = c[0][i];
        }
    }
} prob;

class Solution {
  public:
    Problem prob;
    int cost;
    int heuristic;

    // add solution information here
    int n, x[maxn];

    Solution(Problem &prob) {
        this->prob = prob;
        this->n = prob.n;
        this->cost = VERY_BIG_NUMBER;
        this->heuristic = -VERY_BIG_NUMBER;
    }

    void init() {
        int current = 0;
        bool visited[maxn];
        memset(visited, false, sizeof(visited));
        visited[current] = true;
        for (int i = 1; i < n; ++i) {
            int next = -1;
            for (int j = 0; j < n; ++j) {
                if (!visited[j] && (next == -1 || prob.c[current][j] <
                                                      prob.c[current][next])) {
                    next = j;
                }
            }
            x[i] = next;
            visited[next] = true;
            current = next;
        }
        recalculate_cost();
    }

    bool check_feasible() { return true; }

    void recalculate_cost() {
        this->cost = 0;
        for (int i = 0; i < n; ++i) {
            this->cost += prob.c[x[i]][x[(i + 1) % n]];
        }
    }

    void recalculate_heuristic() {
        // TODO
    }

    void print_debug() {
        cout << "n: " << n << endl;
        cout << "x: ";
        for (int i = 0; i < n; ++i) {
            cout << x[i] << " ";
        }
        cout << endl;
        cout << "cost: " << cost << endl;
        cout << endl;
    }

    void print_answer() {
        cout << n << endl;
        for (int i = 0; i < n; ++i) {
            cout << x[i] + 1 << " ";
        }
    }
};

class FindNeighborOperator {
  public:
    string type = "best"; // maybe change to "accept_worse" in future

    FindNeighborOperator() {}

    FindNeighborOperator(string type) { this->type = type; }

    Solution neighbor(Solution sol, int i, int j) {
        assert(i < j && i > 0);
        Solution newsol = sol;
        update_sol(newsol, i, j);
        update_cost(newsol, i, j);
        return newsol;
    }

    void update_sol(Solution &sol, int i, int j) { swap(sol.x[i], sol.x[j]); }

    void update_cost(Solution &sol, int i, int j) {
        // note that update cost is called after update solution, so we
        // calculate delta = current cost - previous cost
#ifndef NDEBUG
        assert(i < j && i > 0);
        cout << i << "  " << j << endl;
        cout << sol.x[i - 1] << " " << sol.x[i] << " " << sol.x[i + 1] << endl;
        cout << sol.x[j - 1] << " " << sol.x[j] << " " << sol.x[j + 1] << endl;
#endif
        int delta = 0;
        if (i < j - 1)
            delta = -prob.c[sol.x[i - 1]][sol.x[j]] -
                    prob.c[sol.x[j]][sol.x[i + 1]] -
                    prob.c[sol.x[j - 1]][sol.x[i]] -
                    prob.c[sol.x[i]][sol.x[j + 1]] +
                    prob.c[sol.x[i - 1]][sol.x[i]] +
                    prob.c[sol.x[i]][sol.x[i + 1]] +
                    prob.c[sol.x[j - 1]][sol.x[j]] +
                    prob.c[sol.x[j]][sol.x[j + 1]];
        else { // j == i + 1
               //(prevous) i-1 i i+1 i+2 -> (current) i-1 i+1 i i+2
            delta = -prob.c[sol.x[i - 1]][sol.x[i + 1]] -
                    prob.c[sol.x[i + 1]][sol.x[i]] -
                    prob.c[sol.x[i]][sol.x[i + 2]] +
                    prob.c[sol.x[i - 1]][sol.x[i]] +
                    prob.c[sol.x[i]][sol.x[i + 1]] +
                    prob.c[sol.x[i + 1]][sol.x[i + 2]];
        }
#ifndef NDEBUG
        int new_cost = sol.cost + delta;
        // check if the update cost is correct
        sol.recalculate_cost();
        assert(new_cost == sol.cost);
#else
        sol.cost += delta;
#endif
    }

    void update_heuristic(Solution &sol) {
        // TODO
    }

    Solution find_best_neighbor(Solution sol) {
        Solution best = sol;
        for (int i = 1; i < sol.n; ++i) {
            for (int j = i + 1; j < sol.n; ++j) {
                Solution tmp = neighbor(sol, i, j);
                if (tmp.cost < best.cost)
                    best = tmp;
            }
        }
        return best;
    }
};

class LocalSearch {
  public:
    FindNeighborOperator localsearch_operator;
    LocalSearch() {}
    LocalSearch(FindNeighborOperator localsearch_operator) {
        assert(localsearch_operator.type == "best");
        this->localsearch_operator = localsearch_operator;
    }

    Solution search_with_iter(Solution sol) {
        int i = 0;
        while (i++ < MAXITER) {
            Solution neighbor = localsearch_operator.find_best_neighbor(sol);
            if (neighbor.cost < sol.cost) {
                sol = neighbor;
            } else
                break;
        }
        return sol;
    }

    Solution search_with_time(Solution sol) {
        time_t start_time = time(nullptr);
        while (time(nullptr) - start_time < MAXTIME) {
            Solution neighbor = localsearch_operator.find_best_neighbor(sol);
            if (neighbor.cost < sol.cost) {
                sol = neighbor;
            } else
                break;
        }
        return sol;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    prob.input();
    Solution sol(prob);
    sol.init();
    FindNeighborOperator find_neighbor_operator;
    LocalSearch localsearch(find_neighbor_operator);
    sol = localsearch.search_with_time(sol);
    sol.print_answer();

    return 0;
}
