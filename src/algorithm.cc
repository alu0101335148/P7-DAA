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
  return result;
}


/**
 * @brief Function that implements the GRASP algorithm
 * @param max_iterations number of iterations to perform the algorithm
 * @param seed seed to initialize the random number generator
 * @param initial_node initial node to start the route
 * @return Solution object of the Solution class
 */
Solution Algorithm::GRASPSolver(const int max_iterations, const int seed,
                                const int initial_node) {
  srand(seed);
  Solution best_solution(problem_->getNumVehicles());
  int iterations = 0;
  while (iterations < max_iterations) {
    Solution initial_solution = GRC(seed, initial_node);
    Solution sharp_solution = localSearch(initial_solution);
    if (sharp_solution.calculateCost() < best_solution.getCost()) {
      best_solution = sharp_solution;
    }
    iterations++;
  }
  return best_solution;
}


/**
 * @brief Local search function to improve the solution
 * @param initial_solution initial solution to improve
 * @return Solution object of the Solution class
 */
Solution Algorithm::localSearch(Solution initial_solution) {
  return local_search_.run(initial_solution);
}


/**
 * @brief This function implements the constructive phase of GRASP algorithm
 * 
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
