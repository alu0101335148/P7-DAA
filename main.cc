#include "route.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <climits>

typedef std::vector<std::vector<int>> Matrix;

std::pair<int, int> readInitialValues(std::ifstream& file) {
  std::string line = "";

  // Read the first two lines to get the number of clients and vehicles
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

Matrix readDistanceMatrix(std::ifstream& file) {
  std::string line = "";

  // Read the first two lines to get the number of clients and vehicles
  int posFound = 0;
  std::string splitted = "";

  Matrix matrix = {};
  while (std::getline(file, line)) {
    std::string splitted = "";
    std::vector<int> lineSplitted;
    for (int i = 0; i < line.length(); i++) {
      if (line[i] != '\t') {
        splitted += line[i];
      } else {
        lineSplitted.push_back(atoi(splitted.c_str()));
        splitted = "";
      }
    }
    matrix.push_back(lineSplitted);
  }
  return matrix;
}

bool allClientsVisited(std::vector<bool>& visited) {
  for (size_t i = 0; i < visited.size(); i++) {
    if (!visited[i]) return false;
  }
  return true;
}

std::pair<int, int> 
findMinNotVisited(const Matrix& distanceMatrix, 
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


std::vector<Route>
greedySolver(const Matrix& distanceMatrix, const int nVehicles, 
             const int initialNode = 0) {
  std::vector<bool> visitedClients (distanceMatrix.size(), false);
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
      std::pair<int, int> nextClient = findMinNotVisited(distanceMatrix, 
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
 * @brief main function of the problem: this function receives a filename and
 * read it line by line to the end of file
 * @param argc number of arguments
 * @param argv arguments
 * @return 0 if the program ends successfully
 */
int main(int argc, char* argv[]) {
  std::string filename;
  if (argc == 2) {
    filename = argv[1];
  } else {
    std::cout << "Please enter a filename: ";
    std::cin >> filename;
  }
  std::ifstream file(filename);
  if (file.is_open()) {
    std::pair<int, int> initialValues = readInitialValues(file);
    int nClients = initialValues.first;
    int nVehicles = initialValues.second;

    Matrix distanceMatrix = readDistanceMatrix(file);
    file.close();

    std::vector<Route> result = greedySolver(distanceMatrix, nVehicles);

    for (size_t i = 0; i < result.size(); i++) {
      std::cout << "Route " << i << ": ";
      result[i].printRoute();
    }
  } else {
    std::cout << "Error opening file" << std::endl;
    return -1;
  }
  return 0;
}