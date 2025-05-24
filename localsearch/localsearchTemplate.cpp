#define NDEBUG
#include <bits/stdc++.h>
using namespace std;

const int VERY_BIG_NUMBER = 1e8;
const double MAXTIME = 110;
const int MAXITER = 1000;
const int maxn = 405;

class Problem {
  public:
    Problem() {}

    // TODO: add more information

    void input() {
        // TODO
    }
} prob;

class Solution {
  public:
    Problem prob;
    int cost;
    int heuristic;

    // TODO: add more information

    Solution(Problem &prob) {
        this->cost = VERY_BIG_NUMBER;
        this->heuristic = -VERY_BIG_NUMBER;
        // TODO
    }

    void init() {
        // TODO: create an initial solution
        recalculate_cost();
    }

    bool check_feasible() {
        // TODO
        return true;
    }

    void recalculate_cost() {
        // TODO: calculate cost from scratch
    }

    void recalculate_heuristic() {
        // TODO: calculate heuristic from scratch
    }

    void print_debug() {
        // TODO
    }

    void print_answer() {
        // TODO
    }
};

class FindNeighborOperator {
  public:
    FindNeighborOperator() {}

    // TODO: create new constructor if needed

    Solution neighbor(Solution sol) {
        Solution newsol = sol;
        update_sol(newsol);
        update_cost(newsol);
        update_heuristic(newsol);
        return newsol;
    }

    void update_sol(Solution &sol) {
        // TODO: update solution incrementally
    }

    void update_cost(Solution &sol) {
        // TODO: update sol.cost incrementally
    }

    void update_heuristic(Solution &sol) {
        // TODO: update sol.heuristic incrementally
    }

    Solution find_best_neighbor(Solution sol) {
        Solution best = sol;
        // TODO: find best neighbor from all possible neighbors
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
