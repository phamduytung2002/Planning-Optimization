#define NDEBUG
#include <bits/stdc++.h>
using namespace std;

const int VERY_BIG_NUMBER = 1e8;
const int maxn = 405;

class AbstractProblem {
  public:
    AbstractProblem() {}

    // TODO: add more information

    virtual void input() {
        // TODO
    }
} prob;

class AbstractSolution {
  public:
    AbstractProblem prob;
    int cost;
    int heuristic;

    // TODO: add more information

    AbstractSolution(AbstractProblem &prob) {
        this->cost = VERY_BIG_NUMBER;
        this->heuristic = -VERY_BIG_NUMBER;
        // TODO
    }

    virtual void init() {
        // TODO: create an initial solution
        recalculate_cost();
    }

    virtual bool check_feasible() {
        // TODO
        return true;
    }

    virtual void recalculate_cost() {
        // TODO: calculate cost from scratch
    }

    virtual void recalculate_heuristic() {
        // TODO: calculate heuristic from scratch
    }

    virtual void print_debug() {
        // TODO
    }

    virtual void print_answer() {
        // TODO
    }
};

class FindNeighborOperator {
  public:
    FindNeighborOperator() {}

    // TODO: create new constructor if needed

    AbstractSolution neighbor(AbstractSolution sol) {
        // f(sol, params) = newsol; add params
        AbstractSolution newsol = sol;
        update_sol(newsol);
        update_cost(newsol);
        update_heuristic(newsol);
        return newsol;
    }

    void update_sol(AbstractSolution &sol) {
        // TODO: update solution incrementally
    }

    void update_cost(AbstractSolution &sol) {
        // TODO: update sol.cost incrementally
    }

    void update_heuristic(AbstractSolution &sol) {
        // TODO: update sol.heuristic incrementally
    }

    AbstractSolution find_best_neighbor(AbstractSolution sol) {
        AbstractSolution best = sol;
        // TODO: find best neighbor from all possible neighbors
        return best;
    }
};

class StopCriteria {
  public:
    StopCriteria() {}

    virtual bool check_stop_at_end_iter(AbstractSolution sol) {
        // TODO: check and update variables at end of iter
    }
};

class StopCriteriaIter : public StopCriteria {
  public:
    int maxiter = 1000;
    int iter = 0;
    StopCriteriaIter() {}
    StopCriteriaIter(int maxiter) { this->maxiter = maxiter; }

    bool check_stop_at_end_iter(AbstractSolution sol) {
        return iter++ >= maxiter;
    }
};

class StopCriteriaTime : public StopCriteria {
  public:
    int maxtime = 60;
    time_t start_time;
    StopCriteriaTime() { this->start_time = time(nullptr); }
    StopCriteriaTime(int maxtime) {
        this->start_time = time(nullptr);
        this->maxtime = maxtime;
    }

    bool check_stop_at_end_iter(AbstractSolution sol) {
        return time(nullptr) - start_time > maxtime;
    }
};

class LocalSearch {
  public:
    FindNeighborOperator localsearch_operator;
    StopCriteria stopCriteria;
    LocalSearch() {}
    LocalSearch(FindNeighborOperator localsearch_operator,
                StopCriteria stopCriteria) {
        this->localsearch_operator = localsearch_operator;
        this->stopCriteria = stopCriteria;
    }

    AbstractSolution search(AbstractSolution sol) {
        while(stopCriteria.check_stop_at_end_iter(sol)){
            AbstractSolution neighbor =
                localsearch_operator.find_best_neighbor(sol);
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
    AbstractSolution sol(prob);
    sol.init();

    FindNeighborOperator find_neighbor_operator;
    StopCriteriaTime stopCriteria(60);
    LocalSearch localsearch(find_neighbor_operator, stopCriteria);

    sol = localsearch.search(sol);
    sol.print_answer();

    return 0;
}
