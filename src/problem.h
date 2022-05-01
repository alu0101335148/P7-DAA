/**
 * @file problem.h
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief This file contains the declaration of the class Problem.
 * @version 1.0
 * @date 2022-04-19
 */

#ifndef ___PROBLEM___
#define ___PROBLEM___

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<int>> Matrix;
typedef std::pair<int, int> Pair;

/** @brief This class stores the information about the problem */
class Problem {
  private:
    int num_vehicles_ = 0;
    int num_clients_ = 0;
    Matrix distance_matrix_ = {};
  public:
    /**
     * @brief Construct a new Problem object
     * @param num_vehicles 
     * @param num_clients 
     * @param distance_matrix 
     */
    Problem(int num_vehicles, int num_clients, Matrix distance_matrix);

    /**
     * @brief Construct a new Problem object
     * @details this constructor receives a input file stream and reads the
     * information of the problem
     * @param file input file stream
     */
    Problem(std::ifstream& file);

    /** @brief Destroy the Problem object */
    ~Problem() {};

    /**
     * @brief Get the Num Vehicles object
     * @return int 
     */
    int getNumVehicles() {return num_vehicles_;};

    /**
     * @brief Get the Num Clients object
     * @return int 
     */
    int getNumClients() {return num_clients_;};

    /**
     * @brief Get the Distance Matrix object
     * @return Matrix 
     */
    Matrix getDistanceMatrix() {return distance_matrix_;};
};

#endif
