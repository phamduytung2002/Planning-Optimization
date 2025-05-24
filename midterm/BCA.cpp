// #define NDEBUG
#include <bits/stdc++.h>
using namespace std;

const int VERY_BIG_NUMBER = 1e8;
const double MAXTIME = 110;
const int MAXITER = 1000;

typedef pair<int, int> pii;

int tmp;

class Problem {
  public:
    Problem() {}

    int m;       // number of teachers, <= 500
    int n;       // number of courses, <= 1000
    int k[1001]; // number of courses of each teacher, <= 10
    int q;       // number of conflict pair of courses

    vector<int> preference[1001];
    vector<int> preferenced[1001];
    vector<pii> conflicts; // conflict pair of courses
    bool cf[1001][1001];

    void input() {
        cin >> m >> n;
        for (int i = 1; i <= m; ++i) {
            cin >> k[i];
            for (int j = 1; j <= k[i]; ++j) {
                cin >> tmp;
                preference[i].push_back(tmp);
                preferenced[tmp].push_back(i);
            }
        }
        cin >> q;
        for (int i = 0; i < q; ++i) {
            int u, v;
            cin >> u >> v;
            conflicts.push_back(make_pair(u, v));
            cf[u][v] = cf[v][u] = true;
        }
    }
} prob;

class Solution {
  public:
    Problem prob;
    int cost;
    int penalty;

    int n, m;
    int x[1001];                    // x[i] = j: course i is taught by teacher j
    int load[1001];                 // load[i]: load of teacher i
    vector<int> course_teach[1001]; // course_teach[i]: list of courses taught
                                    // by teacher i

    Solution(Problem &prob) {
        this->cost = VERY_BIG_NUMBER;
        this->penalty = -VERY_BIG_NUMBER;
        this->prob = prob;
        this->n = prob.n;
        this->m = prob.m;
    }

    void init() {
        // create an initial solution
        // assign course to teacher who have the least load and can teach that
        // course
        for (int i = 1; i <= n; ++i) {
            int min_load = VERY_BIG_NUMBER;
            int teacher = -1;
            for (int j = 0; j < prob.preferenced[i].size(); ++j) {
                int tmp_teacher = prob.preferenced[i][j];
                bool ok = true;
                if (load[tmp_teacher] >= min_load)
                    ok = false;
                for (int a = 0; a < course_teach[tmp_teacher].size(); ++a) {
                    for (int b = a + 1; b < course_teach[tmp_teacher].size();
                         ++b) {
                        int u = course_teach[tmp_teacher][a];
                        int v = course_teach[tmp_teacher][b];
                        if (prob.cf[u][v]) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok)
                        break;
                }
                if (ok) {
                    teacher = tmp_teacher;
                    min_load = load[tmp_teacher];
                    course_teach->push_back(i);
                }
            }
            x[i] = teacher;
            load[teacher] += 1;
        }
        recalculate_cost();
    }

    bool check_feasible() {
        for (int i = 1; i <= prob.conflicts.size(); ++i) {
            int u = prob.conflicts[i].first;
            int v = prob.conflicts[i].second;
            if (x[u] == x[v])
                return false;
        }
        return true;
    }

    void recalculate_cost() {
        int mi = VERY_BIG_NUMBER;
        int ma = -VERY_BIG_NUMBER;
        for (int i = 1; i <= m; ++i) {
            mi = min(mi, load[i]);
            ma = max(ma, load[i]);
        }
        cost = ma - mi;
    }

    void recalculate_penalty() {
        for (int i = 1; i <= prob.conflicts.size(); ++i) {
            int u = prob.conflicts[i].first;
            int v = prob.conflicts[i].second;
            if (x[u] == x[v])
                penalty += 100;
        }
    }

    void print_debug() {
#ifndef NDEBUG
        cout << "\nload:\n";
        for (int i = 1; i <= m; ++i) {
            cout << load[i] << " ";
        }
        cout << endl;
#endif
    }

    void print_answer() {
        cout << n << endl;
        for (int i = 1; i <= n; ++i) {
            cout << x[i] << " ";
        }
    }
};

class FindNeighborOperator {
  public:
    FindNeighborOperator() {}

    // TODO: create new constructor if needed

    Solution neighbor(Solution sol) {
        Solution newsol = sol;
        return find_best_neighbor(newsol);
    }

    void update_sol(Solution &sol, int course, int teacher) {
        // choose the teacher who have the leaset load and can teach that course

        int min_load = VERY_BIG_NUMBER;
        int course_id = -1;
        int teacher_id = -1;
        for (int i = 1; i <= prob.n; ++i) {
            // choose course
            for (int j = 0; j < prob.preferenced[i].size(); ++j) {
                int teacher = prob.preferenced[i][j];
                if (sol.load[teacher] < min_load) {
                    min_load = sol.load[teacher];
                    course_id = i;
                    teacher_id = teacher;
                }
            }
        }
        sol.x[course_id] = teacher_id;
    }

    void update_cost(Solution &sol) { sol.recalculate_cost(); }

    void update_penalty(Solution &sol) { sol.recalculate_penalty(); }

    Solution find_best_neighbor(Solution sol) {
        Solution best = find_best_neighbor(sol);
        return best;
    }
};

class LocalSearch {
  public:
    FindNeighborOperator localsearch_operator;
    LocalSearch() {}
    LocalSearch(FindNeighborOperator localsearch_operator) {
        this->localsearch_operator = localsearch_operator;
    }

    Solution search_with_iter(Solution sol) {
        int i = 0;
        while (i++ < MAXITER) {
            Solution neighbor = localsearch_operator.find_best_neighbor(sol);
            if (neighbor.cost + neighbor.penalty <
                sol.cost + neighbor.penalty) {
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
            if (neighbor.cost + neighbor.penalty <
                sol.cost + neighbor.penalty) {
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
#ifndef NDEBUG
    freopen("input.txt", "r", stdin);
#endif

    prob.input();
    Solution sol(prob);
    sol.init();
    sol.print_answer();
    sol.print_debug();

    FindNeighborOperator find_neighbor_operator;
    LocalSearch localsearch(find_neighbor_operator);
    sol = localsearch.search_with_time(sol);
    sol.print_answer();

    return 0;
}
