#include "algorithm.h"

/**
 * @brief Implementation of a greedy algorithm to find the best route.
 * @details This function uses a greedy algorithm to find the best route. It
 * starts from the first node and search the node with the lowest cost to go
 * to from the actual node. Then, it adds the node to the route and repeat this
 * process for each vehicle (each route) until we finish to visit all nodes.
 * 
 * @param initialNode initial position to start the route
 * @return std::vector<Route> routes of each vehicle
 */
Solution Algorithm::greedySolver(const int initialNode) {
  std::vector<bool> visitedClients = {};
  visitedClients.resize(problem_->getDistanceMatrix().size(), false);
  Solution solution(problem_->getNumVehicles());
  visitedClients[initialNode] = true;
  int actualNode = initialNode;
  for (size_t i = 0; i < solution.getRoutes().size(); i++) {
    solution.getRoutes()[i].addClient(initialNode);
  }

  while(!allClientsVisited(visitedClients)) {
    for (int i = 0; i < problem_->getNumVehicles(); i++) {
      if(allClientsVisited(visitedClients)) {
        break;
      }
      actualNode = solution.getRoutes()[i].getLastClient();
      Pair nextClient = findMinNotVisited(visitedClients,
                                          actualNode);
      visitedClients[nextClient.first] = true;
      solution.getRoutes()[i].addClient(nextClient.first);
      solution.getRoutes()[i].getCost() += nextClient.second;
    }
  }

  for (int i = 0; i < solution.getRoutes().size(); i++) {
    Matrix distance_matrix = problem_->getDistanceMatrix();
    solution.getRoutes()[i].getCost() += distance_matrix[solution.getRoutes()[i].getLastClient()][initialNode];
    solution.getRoutes()[i].addClient(initialNode);
  }
  return solution;
}

//___________________________________________________________________________//

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
 * @brief This function calculates the probabilities of each avaible node
 * @details This function calculates accumulative probabilities of each avaible
 * node.
 * 
 * @param avaibleClients list of all the avaible clients
 * @param actualNode actual node
 * @return std::vector<float> vector of probabilities
 */
std::vector<float> 
Algorithm::calculateProbabilities(std::vector<int> avaibleClients, 
                                  int actualNode) {
  int sumCost = 0;
  Matrix distance_matrix = problem_->getDistanceMatrix();

  // Sum all the cost
  for (int i = 0; i < avaibleClients.size(); i++) {
    sumCost += distance_matrix[actualNode][avaibleClients[i]];
  }

  // Generate a random number
  std::vector<float> probabilities = {};

  if (avaibleClients.size() == 1) {
    probabilities.push_back(1);
    return probabilities;
  }
  for (int i = 0; i < avaibleClients.size(); i++) {
    int cost = distance_matrix[actualNode][avaibleClients[i]];
    float probability = (1.0 - (float) cost / (float) sumCost);
    probabilities.push_back(probability);
  }

  // Normalize the probabilites
  float sumProbabilities = 0;
  for (int i = 0; i < probabilities.size(); i++) {
    sumProbabilities += probabilities[i];
  }
  for (int i = 0; i < probabilities.size(); i++) {
    probabilities[i] /= sumProbabilities;
  }

  // Calculate the accumulative probabilities
  for (int i = 1; i < avaibleClients.size(); i++) {
    probabilities[i] += probabilities[i - 1];
  }

  return probabilities;
}


/**
 * @brief This function selects a node based on the probabilities calculated
 * @details the probability to go to a node is calculated based on the cost to
 * go to that node.
 * 
 * @param avaibleClients list of avaible clients
 * @param actualNode actual node
 * @return Pair next node and cost to go to that node
 */
Pair Algorithm::findRandomMinNotVisited(std::vector<int> avaibleClients,
                                        int actualNode) {
  std::vector<float> probabilities = calculateProbabilities(avaibleClients, 
                                                            actualNode);
  int newClient = -1;
  Matrix distance_matrix = problem_->getDistanceMatrix();
  float random = ((double) rand() / (INT_MAX));
  for (int i = 0; i < avaibleClients.size(); i++) {
    if (random <= probabilities[i]) {
      newClient = avaibleClients[i];
      break;
    }
  }
  return {newClient, distance_matrix[actualNode][newClient]};
}


/**
 * @brief This function implements the constructive phase of GRASP algorithm
 * 
 * @param seed seed for random number generator
 * @param distanceMatrix matrix of distances
 * @param nVehicles number of vehicles (number of routes)
 * @param nClients number of clients
 * @param initialNode initial position to start the route
 * @return std::vector<Route> vector of routes
 */
Solution Algorithm::GRC(int seed, const int initialNode) {
  Matrix distance_matrix = problem_->getDistanceMatrix();
  srand(seed);
  std::vector<int> avaibleClients = {};
  for (size_t i = 0; i < distance_matrix.size(); i++) {
    avaibleClients.push_back(i);
  }

  Solution solution(problem_->getNumVehicles());
  for (size_t i = 0; i < solution.getRoutes().size(); i++) {
    solution.getRoutes()[i].addClient(initialNode);
  }

  int actualNode = initialNode;
  avaibleClients.erase(avaibleClients.begin() + actualNode);

  while(!avaibleClients.empty()) {    
    for (int i = 0; i < problem_->getNumVehicles(); i++) {
      if(avaibleClients.empty()) {
        break;
      }
      actualNode = solution.getRoutes()[i].getLastClient();
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
      solution.getRoutes()[i].addClient(nextClient.first);
      solution.getRoutes()[i].getCost() += nextClient.second;
    }
  }

  for (int i = 0; i < solution.getRoutes().size(); i++) {
    solution.getRoutes()[i].getCost() += distance_matrix[solution.getRoutes()[i].getLastClient()][initialNode];
    solution.getRoutes()[i].addClient(initialNode);
  }
  return solution;
}
