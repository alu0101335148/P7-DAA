/**
 * @file local_search.cc
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief File that contains the implementation of the class LocalSearch.
 * @version 0.1
 * @date 2022-04-25
 */

#include "local_search.h"

/** @brief Construct a new Local Search:: Local Search object */
LocalSearch::LocalSearch() {
  problem_ = NULL;
}

/** @brief Destroy the Local Search:: Local Search object */
LocalSearch::~LocalSearch() {}

/**
 * @brief Set the Problem object
 * @param problem problem object
 */
void LocalSearch::setProblem(Problem* problem) {
  problem_ = problem;
}

/**
 * @brief Function that chooses the local search algorithm
 * @param initial_solution 
 * @return Solution 
 */
Solution LocalSearch::run(Solution initial_solution) {
  return swapIntraRoute(initial_solution);
  // return swapInterRoute(initial_solution);
  return reinsertionIntraRoute(initial_solution);
  return reinsertionInterRoute(initial_solution);
}

//------------------------------SWAP-INTRA-ROUTE-----------------------------//

/**
 * @brief LocalSearch by swap intra-route
 * @param initial_solution 
 * @return Solution 
 */
Solution LocalSearch::swapIntraRoute(Solution initial_solution) {
  std::vector<Route> routes = initial_solution.getRoutes();
  for (int i = 0; i < routes.size(); i++) {
    intraRouteSwapProcedure(routes[i]);
  }
  return Solution(routes);
}

/**
 * @brief Procesure to swap intra-route
 * @param route route to improve
 */
void LocalSearch::intraRouteSwapProcedure(Route& route) {
  int best_cost = route.getCost();
  int first_index = -1;
  int second_index = -1;
  bool improved = false;
  do {
    improved = false;
    for (int i = 1; i < route.getSize() - 1; i++) {
      for (int j = i + 1; j < route.getSize() - 1; j++) {
        int cost_of_swap = swapCost(i, j, route);
        if (cost_of_swap < best_cost) {
          best_cost = cost_of_swap;
          first_index = i;
          second_index = j;
        }
      }
    }
    if (first_index != -1 && second_index != -1) {
      route.swap(first_index, second_index);
      route.getCost() = best_cost;
      first_index = -1;
      second_index = -1;
      improved = true;
    }
  } while (improved);
}

/**
 * @brief Axiliar function to swap cost
 * @param first_index 
 * @param second_index 
 * @param route 
 * @return int cost of the swap
 */
int LocalSearch::swapCost(int first_index, int second_index, Route route) {
  Matrix distance_matrix = problem_->getDistanceMatrix();
  int first_value = route[first_index];
  int second_value = route[second_index];
  int first_value_previus = route[first_index - 1];
  int first_value_next = route[first_index + 1];
  int second_value_previus = route[second_index - 1];
  int second_value_next = route[second_index + 1];
  int cost_of_swap = route.getCost()
  - distance_matrix[first_value_previus][first_value]
  - distance_matrix[first_value][first_value_next]
  - distance_matrix[second_value][second_value_next]
  + distance_matrix[first_value_previus][second_value]
  + distance_matrix[first_value][second_value_next];
  if (second_index - first_index == 1) {
    cost_of_swap += distance_matrix[second_value][first_value];
  } else {
    cost_of_swap = cost_of_swap
    - distance_matrix[second_value_previus][second_value]
    + distance_matrix[second_value_previus][first_value]
    + distance_matrix[second_value][first_value_next];
  }
  return cost_of_swap;
}


//----------------------------SWAP-INTER-ROUTE-------------------------------//

/**
 * @brief local search by swap inter-route
 * @param initial_solution 
 * @return Solution 
 */
Solution LocalSearch::swapInterRoute(Solution initial_solution) {
  std::vector<Route> routes = initial_solution.getRoutes();
  for (int i = 0; i < routes.size(); i++) {
    for (int j = i + 1; j < routes.size(); j++) {
      interRouteSwapProcedure(routes[i], routes[j]);
    }
  }
  return Solution(routes);
}

/**
 * @brief Auxiliar method to implement the inter route swap procedure
 * @param first_route 
 * @param second_route 
 */
void LocalSearch::interRouteSwapProcedure(Route& first_route, Route& second_route) {
  // Set the best cost to the initial cost
  std::pair<int, int> best_cost = {first_route.getCost(), second_route.getCost()};
  int first_index = -1;
  int second_index = -1;

  bool improved = false;
  do {
    improved = false;
    for (int i = 1; i < first_route.getSize() - 1; i++) {
      for (int j = 1; j < second_route.getSize() - 1; j++) {
        std::pair<int, int> cost_of_swap = swapCost(i, j, first_route, second_route);
        if ((cost_of_swap.first + cost_of_swap.second) < (best_cost.first + best_cost.second)) {
          best_cost = cost_of_swap;
          first_index = i;
          second_index = j;
        }
      }
    }
    if (first_index != -1 && second_index != -1) {
      int temp = first_route[first_index];
      first_route[first_index] = second_route[second_index];
      second_route[second_index] = temp;
      first_route.getCost() = best_cost.first;
      second_route.getCost() = best_cost.second;
      first_index = -1;
      second_index = -1;
      improved = true;
    }
  } while (improved);
} 

/**
 * @brief Auxiliar method to calculate the cost of the swap
 * @param first_index 
 * @param second_index 
 * @param first_route 
 * @param second_route 
 * @return std::pair<int,int> 
 */
std::pair<int,int> LocalSearch::swapCost(int first_index, int second_index,
                                       Route first_route, Route second_route) {
  Matrix distance_matrix = problem_->getDistanceMatrix();
  int first_value_previus = first_route[first_index - 1];
  int second_value_previus = second_route[second_index - 1];
  int first_value_next = first_route[first_index + 1];
  int second_value_next = second_route[second_index + 1];
  int first_value = first_route[first_index];
  int second_value = second_route[second_index];

  int new_first_cost = first_route.getCost()
  - distance_matrix[first_value_previus][first_value]
  - distance_matrix[first_value][first_value_next]
  + distance_matrix[first_value_previus][second_value]
  + distance_matrix[second_value][first_value_next];

  int new_second_cost = second_route.getCost()
  - distance_matrix[second_value_previus][second_value]
  - distance_matrix[second_value][second_value_next]
  + distance_matrix[second_value_previus][first_value]
  + distance_matrix[first_value][second_value_next];

  return {new_first_cost, new_second_cost};
}


//--------------------------REINSERTION-INTRA-ROUTE-------------------------//

/**
 * @brief Implementation of the local search by reinsertion intra-route
 * @param initial_solution solution to be improved
 * @return Solution improved solution
 */
Solution LocalSearch::reinsertionIntraRoute(Solution initial_solution) {
  std::vector<Route> routes = initial_solution.getRoutes();
  for (int i = 0; i < routes.size(); i++) {
    intraRouteReinsertionProcedure(routes[i]);
  }
  return Solution(routes);
}

/**
 * @brief Method to implement the intra route reinsertion procedure
 * @param route route to be improved
 */
void LocalSearch::intraRouteReinsertionProcedure(Route& route) {
  int best_cost = route.getCost();
  int first_index = -1;
  int second_index = -1;
  int counter = 0;

  bool improved = false;
  do {
    improved = false;
    for (int i = 1; i < route.getSize() - 1; i++) {
      for (int j = 0; j < route.getSize() - 1; j++) {
        if (i == j) {continue;}
        if (i == (j + 1)) {continue;}
        int reins_cost = reinsertionCost(i, j, route);
        if (reins_cost < best_cost) {
          best_cost = reins_cost;
          first_index = i;
          second_index = j;
        }
      }
    }
    if (first_index != -1 && second_index != -1) {
      route.Displace(first_index, second_index);
      route.getCost() = best_cost;
      first_index = -1;
      second_index = -1;
      improved = true;
    }
  } while (improved);
}

/**
 * @brief Auxiliar function to calculate the cost of the reinsertion
 * @param first_index
 * @param second_index 
 * @param route
 * @return int 
 */
int LocalSearch::reinsertionCost(int first_index, int second_index, Route route) {
  Matrix distance_matrix = problem_->getDistanceMatrix();
  int first_value = route[first_index];
  int second_value = route[second_index];
  int first_value_previus = route[first_index - 1];
  int first_value_next = route[first_index + 1];
  int second_value_next = route[second_index + 1];
  return route.getCost()
    - distance_matrix[first_value_previus][first_value]
    - distance_matrix[first_value][first_value_next]
    - distance_matrix[second_value][second_value_next]
    + distance_matrix[first_value_previus][first_value_next]
    + distance_matrix[second_value][first_value]
    + distance_matrix[first_value][second_value_next];
}


//--------------------------REINSERTION-INTER-ROUTE-------------------------//

/**
 * @brief Implementation of the local search by reinsertion inter-route
 * @param initial_solution solution to be improved
 * @return Solution improved solution
 */
Solution LocalSearch::reinsertionInterRoute(Solution initial_solution) {
  std::vector<Route> routes = initial_solution.getRoutes();
  for (int i = 0; i < routes.size(); i++) {
    for (int j = i + 1; j < routes.size(); j++) {
      interRouteReinsertionProcedure(routes[i], routes[j]);
    }
  }
  return Solution(routes);
}

/**
 * @brief Implementation of the inter route reinsertion procedure
 * @param first_route 
 * @param second_route 
 */
void LocalSearch::interRouteReinsertionProcedure(Route& first_route, Route& second_route) {
  int upper_limit = (problem_->getNumClients() / problem_->getNumVehicles());
  upper_limit += (problem_->getNumClients() / 10);
  std::pair<int, int> best_cost = {first_route.getCost(), second_route.getCost()};
  int first_index = -1;
  int second_index = -1;

  bool improved = false;
  do {
    if (first_route.getSize() <= 4) {break;}
    if (second_route.getSize() >= upper_limit) {break;}
    improved = false;
    for (int i = 1; i < first_route.getSize() - 1; i++) {
      for (int j = 0; j < second_route.getSize() - 1; j++) {
        std::pair<int, int> cost_of_swap = reinsertionCost(i, j, first_route, second_route);
        if ((cost_of_swap.first + cost_of_swap.second) < (best_cost.first + best_cost.second)) {
          best_cost = cost_of_swap;
          first_index = i;
          second_index = j;
        }
      }
    }
    if (first_index != -1 && second_index != -1) {
      second_route.insert(second_index, first_route.remove(first_index));
      first_route.getCost() = best_cost.first;
      second_route.getCost() = best_cost.second;
      first_index = -1;
      second_index = -1;
      improved = true;
    }
  } while (improved);
}

/**
 * @brief Auxiliar function to get the reinsertion cost
 * @param first_index 
 * @param second_index 
 * @param first_route 
 * @param second_route 
 * @return std::pair<int,int> cost for each changed routed
 */
std::pair<int,int> LocalSearch::reinsertionCost(int first_index, int second_index,
                                                Route first_route, Route second_route) {
  Matrix distance_matrix = problem_->getDistanceMatrix();
  int first_value_previus = first_route[first_index - 1];
  int second_value_previus = second_route[second_index - 1];
  int first_value_next = first_route[first_index + 1];
  int second_value_next = second_route[second_index + 1];
  int first_value = first_route[first_index];
  int second_value = second_route[second_index];

  first_route.getCost() = first_route.getCost()
  - distance_matrix[first_value_previus][first_value]
  - distance_matrix[first_value][first_value_next]
  + distance_matrix[first_value_previus][first_value_next];

  second_route.getCost() = second_route.getCost()
  - distance_matrix[second_value][second_value_next]
  + distance_matrix[second_value][first_value]
  + distance_matrix[first_value][second_value_next];

  return {first_route.getCost(), second_route.getCost()};
}
