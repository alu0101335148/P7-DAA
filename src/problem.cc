/**
 * @file problem.cc
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief File that contains the definition of the problem class methods
 * @version 0.1
 * @date 2022-04-23
 */

#include "problem.h"

Problem::Problem(int num_vehicles, int num_clients, Matrix distance_matrix) {
  num_vehicles_ = num_vehicles;
  num_clients_ = num_clients;
  distance_matrix_ = distance_matrix;
}


Problem::Problem(std::ifstream& file) {
  std::string line = "";
  int posFound = 0;
  std::string splitted = "";

  // Read the first value (number of clients)
  std::getline(file, line);
  posFound = line.find(":\t", 0);
  splitted = line.substr(posFound + 2, line.length() - 1);

  num_clients_ = std::stoi(splitted) + 1;

  // Read the second value (number of vehicles)
  std::getline(file, line);
  posFound = line.find(":\t", 0);
  splitted = line.substr(posFound + 2, line.length() - 1);

  num_vehicles_ = std::stoi(splitted);

  // Separation between the two values and the matrix in the file
  std::getline(file, line);

  // Read the matrix
  Matrix matrix = {};
  matrix.resize(num_clients_);
  for (int i = 0; i < num_clients_; i++) {
    matrix[i].resize(num_clients_);
  }
  int i = 0;
  int j = 0;
  line = "";
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
  distance_matrix_ = matrix;
}

