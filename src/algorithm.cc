/**
 * @file algorithm.cc
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief File that contains the definition of the Algorithm class methods
 * @version 0.1
 * @date 2022-04-23
 */

#include "algorithm.h"

/**
 * @brief Implementation of a greedy algorithm to find the best route.
 * @details This function uses a greedy algorithm to find the best route. It
 * starts from the first node and search the node with the lowest cost to go
 * to from the actual node. Then, it adds the node to the route and repeat this
 * process for each vehicle (each route) until we finish to visit all nodes.
 * 
 * @param initialNode initial position to start the route
 * @return Solution object of the result class
 */
Solution Algorithm::greedySolver(const int initialNode) {
  std::vector<bool> visitedClients = {};
  visitedClients.resize(problem_->getDistanceMatrix().size(), false);
  Solution result(problem_->getNumVehicles());
  visitedClients[initialNode] = true;
  int actualNode = initialNode;
  for (size_t i = 0; i < result.getRoutes().size(); i++) {
    result.getRoutes()[i].addClient(initialNode);
  }

  while(!allClientsVisited(visitedClients)) {
    for (int i = 0; i < problem_->getNumVehicles(); i++) {
      if(allClientsVisited(visitedClients)) {
        break;
      }
      actualNode = result.getRoutes()[i].getLastClient();
      Pair nextClient = findMinNotVisited(visitedClients,
                                          actualNode);
      visitedClients[nextClient.first] = true;
      result.getRoutes()[i].addClient(nextClient.first);
      result.getRoutes()[i].getCost() += nextClient.second;
    }
  }

  for (int i = 0; i < result.getRoutes().size(); i++) {
    Matrix distance_matrix = problem_->getDistanceMatrix();
    result.getRoutes()[i].getCost() += distance_matrix[result.getRoutes()[i].getLastClient()][initialNode];
    result.getRoutes()[i].addClient(initialNode);
  }
  result.calculateCost();
  return result;
}


/**
 * @brief Function that implements the GRASP algorithm
 * @details This function implements the GRASP algorithm. It starts from the
 * initial solution and search the best solution by applying the local search
 * algorithm. Then, it repeat this process for a limited number of times, and
 * return the best solution found.
 * @param max_iterations number of iterations to perform the algorithm
 * @param seed seed to initialize the random number generator
 * @param initial_node initial node to start the route
 * @return Solution object of the Solution class
 */
Solution Algorithm::GRASPSolver(const int max_iterations, const int seed, 
                                int local_search, const int initial_node) {
  srand(seed);
  Solution best_solution(problem_->getNumVehicles());
  int iterations = 0;
  while (iterations < max_iterations) {
    Solution initial_solution = GRC(seed, initial_node);
    Solution sharp_solution = localSearch(initial_solution, local_search);
    if (sharp_solution.calculateCost() < best_solution.getCost()) {
      best_solution = sharp_solution;
    }
    iterations++;
  }
  return best_solution;
}



/** 
 * @brief Implementation of the GVNS algorithm
 * @param initial_node initial node to start the route
 * @return Solution object of the Solution class
 */
Solution Algorithm::GVNSSolver(const int initial_node) {
  Solution initial_solution = GRC(rand());
  Solution best_solution = initial_solution;

  int counter = 0;
  while(counter < GRASP_ITERATIONS_LIMIT) {
    int k_value = 1;
    while(k_value <= GVNS_K_VALUE_LIMIT) {
      Solution shaked_solution = ShakingSolution(best_solution, k_value);
      shaked_solution.calculateCost();
      Solution sharp_solution = GVNSProcedure(shaked_solution);
      if (sharp_solution.calculateCost() < best_solution.getCost()) {
        //std::cout << best_solution.getCost() << " " << sharp_solution.getCost() <<  std::endl; 
        best_solution = sharp_solution;
        k_value = 1;
      } else {
        k_value++;
      }
    }
    counter++;
  }
  best_solution.calculateCost();
  return best_solution;
}


/**
 * @brief Mehtod to shake an initial solution
 * @details This method reinsert randomly the clients of the initial solution.
 * NOTE: This method limit the size of the routes and the movements, to avoid
 * repeating movements
 * @param initial_solution 
 * @param k_value 
 * @return Solution 
 */
Solution Algorithm::ShakingSolution(Solution initial_solution, 
                                    const int k_value) {
  std::vector<Route> routes = initial_solution.getRoutes();
  int second_route_size = 0;
  int upper_limit = 0;
  std::vector<std::vector<int>> movements = {};

  for (size_t i = 0; i < k_value; i++) {
    int first_route_index = -1;
    int second_route_index = -1;
    bool valid_operation = false;
    do {
      first_route_index = rand() % routes.size();
      second_route_index = rand() % routes.size();
      second_route_size = routes[second_route_index].getSize();

      // upper limit to the number of clients per route
      upper_limit = ((problem_->getNumClients() - 1) / problem_->getNumVehicles());
      upper_limit += (problem_->getNumClients() / 10) + 2;

      if (first_route_index == second_route_index || 
          routes[first_route_index].getSize() <= 4 ||
          second_route_size > upper_limit) {
        continue;
      }
      valid_operation = true;
    } while (!valid_operation);
    int convertion = 0;

    convertion = (routes[first_route_index].getSize() - 2);
    int new_first_route_index = rand() % convertion + 1;
    
    convertion = (routes[second_route_index].getSize() - 1);
    int new_second_route_index = rand() % convertion;

    std::vector<int> actual_movement = {
      first_route_index, 
      new_first_route_index, 
      second_route_index, 
      new_second_route_index
    };

    if (std::find(movements.begin(), movements.end(), actual_movement) != movements.end()) {
      i--;
      continue;
    } else {
      movements.push_back(actual_movement);
    }

    Pair cost_of_relocation = local_search_.reinsertionCost(
      new_first_route_index,
      new_second_route_index,
      routes[first_route_index],
      routes[second_route_index]
    );

    int client = routes[first_route_index].remove(new_first_route_index);
    routes[second_route_index].insert(new_second_route_index, client);
    routes[first_route_index].getCost() = cost_of_relocation.first;
    routes[second_route_index].getCost() = cost_of_relocation.second;
  }
  return Solution(routes);
}


/**
 * @brief Implementation of the GVNS procedure
 * @details This function implements the GVNS procedure. It starts from the
 * initial solution and search the best solution by applying all the the local
 * search algorithm until all the local searchs returns the same solution, 
 * being a local minimun. (repeat until the solution is not improved)
 * NOTE Ther order of local search is the following:
 * 1. Intra-Route-Reinsertion
 * 2. Inter-Route-Reinsertion
 * 3. Intra-Route-Swap
 * 4. Inter-Route-Swap
 * 5. 2-opt
 * @return Solution object of the Solution class
 */
Solution Algorithm::GVNSProcedure(Solution initial_solution) {
  int local_searchs_finished = 0;
  Solution actual_solution = initial_solution;
  Solution next_solution = initial_solution;
  Solution best_solution = initial_solution;
  bool improved = false;
  do {
    improved = false;
    local_searchs_finished = 0;
    do {
      switch (local_searchs_finished) {
        case 0:
          next_solution = local_search_.reinsertionIntraRoute(actual_solution);
          break;
        case 1:
          next_solution = local_search_.reinsertionInterRoute(actual_solution);
          break;
        case 2:
          next_solution = local_search_.swapIntraRoute(actual_solution);
          break;
        case 3:
          next_solution = local_search_.swapInterRoute(actual_solution);
          break;
        case 4:
          next_solution = local_search_.twoOpt(actual_solution);
          break;
        default:
          break;
      }
      if (next_solution.getCost() < actual_solution.getCost()) {
        actual_solution = next_solution;
        local_searchs_finished = 0;
      } else {
        local_searchs_finished++;
      }
    } while (local_searchs_finished < 4);
    if (actual_solution.getCost() < best_solution.getCost()) {
      best_solution = actual_solution;
      improved = true;
    }
  } while (improved);
  return best_solution;
}


/**
 * @brief Local search function to improve the solution
 * @param initial_solution initial solution to improve
 * @return Solution object of the Solution class
 */
Solution Algorithm::localSearch(Solution initial_solution, int local_search) {
  return local_search_.run(initial_solution, local_search);
}


/**
 * @brief This function implements the constructive phase of GRASP algorithm
 * @details Variation of the Greedy algorith, it selects a random node of the
 * best n nodes rather than the best node. 
 * @param seed seed for random number generator
 * @param initialNode initial position to start the route
 * @return Solution Object of the result class
 */
Solution Algorithm::GRC(int seed, const int initialNode) {
  Matrix distance_matrix = problem_->getDistanceMatrix();
  srand(seed);
  std::vector<int> avaibleClients = {};
  for (size_t i = 0; i < distance_matrix.size(); i++) {
    avaibleClients.push_back(i);
  }

  Solution result(problem_->getNumVehicles());
  for (size_t i = 0; i < result.getRoutes().size(); i++) {
    result.getRoutes()[i].addClient(initialNode);
  }

  int actualNode = initialNode;
  avaibleClients.erase(avaibleClients.begin() + actualNode);

  while(!avaibleClients.empty()) {    
    for (int i = 0; i < problem_->getNumVehicles(); i++) {
      if(avaibleClients.empty()) {
        break;
      }
      actualNode = result.getRoutes()[i].getLastClient();
      Pair nextClient = findRandomMinNotVisited(avaibleClients,
                                                actualNode);
      if (nextClient.first == -1) {
        break;
      }
      for (size_t i = 0; i < avaibleClients.size(); i++) {
        if (avaibleClients[i] == nextClient.first) {
          avaibleClients.erase(avaibleClients.begin() + i);
          break;
        }
      }
      result.getRoutes()[i].addClient(nextClient.first);
      result.getRoutes()[i].getCost() += nextClient.second;
    }
  }

  for (int i = 0; i < result.getRoutes().size(); i++) {
    result.getRoutes()[i].getCost() += distance_matrix[result.getRoutes()[i].getLastClient()][initialNode];
    result.getRoutes()[i].addClient(initialNode);
  }
  result.calculateCost();
  return result;
}



/**
 * @brief Functions that check if all the nodes are visited or not.
 * @details true visited, false not visited
 * @param visited vector of visited nodes
 * @return true All visited
 * @return false At least one not visited
 */
bool Algorithm::allClientsVisited(const std::vector<bool>& visited) {
  for (size_t i = 0; i < visited.size(); i++) {
    if (!visited[i]) return false;
  }
  return true;
}


/**
 * @brief This funciton, given a vector of visited nodes, the matrix of 
 * distance and the actual node, returns the index of the next node to visit
 * and the cost to go to that node.
 * @details This funtion search the node with the lowest cost to go to from the
 * actual node.
 *
 * @param visited list of visited nodes
 * @param current actual node
 * @return Pair next node and cost to go to that node
 */
Pair Algorithm::findMinNotVisited(const std::vector<bool>& visited,
                                  const int& current) {
  int min = INT_MAX;
  int minIndex = -1;
  Matrix distance_matrix = problem_->getDistanceMatrix();
  for (size_t i = 0; i < visited.size(); i++) {
    if(visited[i] || i == current) {continue;}
    if (distance_matrix[current][i] < min) {
      min = distance_matrix[current][i];
      minIndex = i;
    }
  }
  return {minIndex, min};
}


/**
 * @brief This function selects a random node to visit and returns the index
 * @details First, it select the best n candidates to visit. Then, selects one
 * randomly
 * 
 * @param avaibleClients list of avaible clients
 * @param actualNode actual node
 * @return Pair next node and cost to go to that node
 */
Pair Algorithm::findRandomMinNotVisited(std::vector<int> avaible_clients,
                                        int actual_node, int candidates) {
  Matrix distance_matrix = problem_->getDistanceMatrix();

  // Select the best n candidates of the avaible clients
  std::vector<int> selected_nodes;
  if (avaible_clients.size() < candidates) {
    selected_nodes = avaible_clients;
  } else {
    for (size_t i = 0; i < candidates; i++) {
      int node_index = 0;
      int minimum_cost = INT_MAX;
      for (size_t j = 0; j < avaible_clients.size(); j++) {
        int cost = distance_matrix[actual_node][avaible_clients[j]];
        if (cost < minimum_cost) {
          minimum_cost = cost;
          node_index = j;
        }
      }
      selected_nodes.emplace_back(avaible_clients[node_index]);
      avaible_clients.erase(avaible_clients.begin() + node_index);
    }
  }    
  // Select a random number of the best candidates
  int newClient = selected_nodes[rand() % selected_nodes.size()];
  return {newClient, distance_matrix[actual_node][newClient]};
}
