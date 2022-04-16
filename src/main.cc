/**
 * @file main.cc
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief Main file for the project.
 * @version 1.0.5
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "route.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <climits>
#include <sstream>
#include <algorithm>

typedef std::vector<std::vector<int>> Matrix;
typedef std::pair<int, int> Pair;

/**
 * @brief This function reads the first two lines of a inpu file and returns a
 * pair with the number of clients and the number of routes.
 * 
 * @param file input file stream
 * @return Pair {number of client, number of routes}
 */
Pair readInitialValues(std::ifstream& file) {
  std::string line = "";

  int posFound = 0;
  std::string splitted = "";

  std::getline(file, line);
  posFound = line.find(":\t", 0);
  splitted = line.substr(posFound + 2, line.length() - 1);
  int nClients = std::stoi(splitted) + 1;

  std::getline(file, line);
  posFound = line.find(":\t", 0);
  splitted = line.substr(posFound + 2, line.length() - 1);
  int nVehicles = std::stoi(splitted);

  std::getline(file, line);
  return {nClients, nVehicles};
}


/**
 * @brief This function reads the matrix of distances from the input file and
 * returns it.
 * 
 * @param file input file stream 
 * @param size size of the matrix
 * @return Matrix distance matrix
 */
Matrix readDistanceMatrix(std::ifstream& file, int size) {
  Matrix matrix = {};
  matrix.resize(size);
  for (int i = 0; i < size; i++) {
    matrix[i].resize(size);
  }
  int i = 0;
  int j = 0;
  std::string line;
  while (!file.eof()) {
    getline(file, line);
    if (line.empty()) {
      continue;
    }
    j = 0;
    std::stringstream ss(line);
    std::string token;
    while (ss >> token) {
      matrix[i][j] = std::stoi(token);
      j++;
    }
    i++;
  }
  return matrix;
}


/**
 * @brief Functions that check if all the nodes are visited or not.
 * @details true visited, false not visited
 * @param visited vector of visited nodes
 * @return true All visited
 * @return false At least one not visited
 */
bool allClientsVisited(const std::vector<bool>& visited) {
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
 * @param distanceMatrix distance matrix
 * @param visited list of visited nodes
 * @param current actual node
 * @return Pair next node and cost to go to that node
 */
Pair findMinNotVisited(const Matrix& distanceMatrix, 
                       const std::vector<bool>& visited,
                       const int& current) {
  int min = INT_MAX;
  int minIndex = -1;
  for (size_t i = 0; i < visited.size(); i++) {
    if(visited[i] || i == current) {continue;}
    if (distanceMatrix[current][i] < min) {
      min = distanceMatrix[current][i];
      minIndex = i;
    }
  }
  return {minIndex, min};
}


/**
 * @brief Implementation of a greedy algorithm to find the best route.
 * @details This function uses a greedy algorithm to find the best route. It
 * starts from the first node and search the node with the lowest cost to go
 * to from the actual node. Then, it adds the node to the route and repeat this
 * process for each vehicle (each route) until we finish to visit all nodes.
 * 
 * @param distanceMatrix distance matrix
 * @param nVehicles number of vehicles (number of routes)
 * @param initialNode initial position to start the route
 * @return std::vector<Route> routes of each vehicle
 */
std::vector<Route> greedySolver(const Matrix& distanceMatrix,
                                const int nVehicles,
                                const int initialNode = 0) {
  std::vector<bool> visitedClients = {};
  visitedClients.resize(distanceMatrix.size(), false);
  std::vector<Route> routes(nVehicles);
  visitedClients[initialNode] = true;
  int actualNode = initialNode;
  for (size_t i = 0; i < routes.size(); i++) {
    routes[i].addClient(initialNode);
  }

  while(!allClientsVisited(visitedClients)) {
    for (int i = 0; i < nVehicles; i++) {
      if(allClientsVisited(visitedClients)) {
        break;
      }
      actualNode = routes[i].getLastClient();
      Pair nextClient = findMinNotVisited(distanceMatrix, 
                                          visitedClients,
                                          actualNode);
      visitedClients[nextClient.first] = true;
      routes[i].addClient(nextClient.first);
      routes[i].getCost() += nextClient.second;
    }
  }

  for (int i = 0; i < routes.size(); i++) {
    routes[i].getCost() += distanceMatrix[routes[i].getLastClient()][initialNode];
    routes[i].addClient(initialNode);
  }
  return routes;
}


/**
 * @brief This function calculates the probabilities of each avaible node
 * @details This function calculates accumulative probabilities of each avaible
 * node.
 * 
 * @param distanceMatrix matrix distance
 * @param avaibleClients list of all the avaible clients
 * @param actualNode actual node
 * @return std::vector<float> vector of probabilities
 */
std::vector<float> calculateProbabilities(const Matrix& distanceMatrix, 
                                          std::vector<int> avaibleClients, 
                                          int actualNode) {
  int sumCost = 0;

  // Sum all the cost
  for (int i = 0; i < avaibleClients.size(); i++) {
    sumCost += distanceMatrix[actualNode][avaibleClients[i]];
  }

  // Generate a random number
  std::vector<float> probabilities = {};

  if (avaibleClients.size() == 1) {
    probabilities.push_back(1);
    return probabilities;
  }
  for (int i = 0; i < avaibleClients.size(); i++) {
    int cost = distanceMatrix[actualNode][avaibleClients[i]];
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
 * @param distanceMatrix matrix of distances 
 * @param avaibleClients list of avaible clients
 * @param actualNode actual node
 * @return Pair next node and cost to go to that node
 */
Pair findRandomMinNotVisited(const Matrix& distanceMatrix, 
                             std::vector<int> avaibleClients, 
                             int actualNode) {
  std::vector<float> probabilities = calculateProbabilities(distanceMatrix, 
                                                            avaibleClients, 
                                                            actualNode);
  int newClient = -1;
  float random = ((double) rand() / (INT_MAX));
  for (int i = 0; i < avaibleClients.size(); i++) {
    if (random <= probabilities[i]) {
      newClient = avaibleClients[i];
      break;
    }
  }
  return {newClient, distanceMatrix[actualNode][newClient]};
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
std::vector<Route> GRC(int seed, const Matrix& distanceMatrix, 
                       const int nVehicles, const int nClients,
                       const int initialNode = 0) {
  std::vector<int> avaibleClients = {};
  for (size_t i = 0; i < distanceMatrix.size(); i++) {
    avaibleClients.push_back(i);
  }

  std::vector<Route> routes(nVehicles);
  for (size_t i = 0; i < routes.size(); i++) {
    routes[i].addClient(initialNode);
  }

  int actualNode = initialNode;
  avaibleClients.erase(avaibleClients.begin() + actualNode);

  while(!avaibleClients.empty()) {    
    for (int i = 0; i < nVehicles; i++) {
      if(avaibleClients.empty()) {
        break;
      }
      actualNode = routes[i].getLastClient();
      Pair nextClient = findRandomMinNotVisited(distanceMatrix, 
                                                avaibleClients,
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
      routes[i].addClient(nextClient.first);
      routes[i].getCost() += nextClient.second;
    }
  }

  for (int i = 0; i < routes.size(); i++) {
    routes[i].getCost() += distanceMatrix[routes[i].getLastClient()][initialNode];
    routes[i].addClient(initialNode);
  }
  return routes;
}


/**
 * @brief main function of the problem: this function receives a filename and
 * read it line by line to the end of file
 * @param argc number of arguments
 * @param argv arguments
 * @return 0 if the program ends successfully
 */
int main(int argc, char* argv[]) {
  std::string filename = "";
  if (argc == 2) {
    filename = argv[1];
  } else {
    std::cout << "Please enter a filename: ";
    std::cin >> filename;
  }
  std::ifstream file(filename);
  if (file.is_open()) {
    Pair initialValues = readInitialValues(file);
    int nClients = initialValues.first;
    int nVehicles = initialValues.second;

    Matrix distanceMatrix = readDistanceMatrix(file, nClients);
    file.close();

    std::vector<Route> result = greedySolver(distanceMatrix, nVehicles);
    std::cout << "Normal Greedy:\n";
    for (size_t i = 0; i < result.size(); i++) {
      std::cout << "Route " << i << ": ";
      result[i].printRoute();
    }
    std::vector<Route> constructiveResult = GRC(0, distanceMatrix, nVehicles, nClients);
    std::cout << "\nConstructive:\n";
    for (size_t i = 0; i < constructiveResult.size(); i++) {
      std::cout << "Route " << i << ": ";
      constructiveResult[i].printRoute();
    }

  } else {
    std::cout << "Error opening file\n";
    return -1;
  }
  return 0;
}
