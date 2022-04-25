/**
 * @file main.cc
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief Main file for the project.
 * @version 1.0.5
 * @date 2022-04-15
 */

#include "algorithm.h"

#include <ctime>

/**
 * @brief main function of the problem: this function receives a filename and
 * read it line by line to the end of file
 * @param argc number of arguments
 * @param argv arguments
 * @return 0 if the program ends successfully
 */
int main(int argc, char* argv[]) {
  srand(std::time(NULL));
  std::string filename = "";
  if (argc == 2) {
    filename = argv[1];
  } else {
    std::cout << "Please enter a filename: ";
    std::cin >> filename;
  }
  std::ifstream file(filename);
  if (file.is_open()) {
    Problem problem(file);
    file.close();
    Algorithm algorithm(&problem);

    std::cout << "Normal Greedy:\n";
    algorithm.greedySolver().printSolution();

    std::cout << "\nConstructive:\n";
    algorithm.GRC(rand()).printSolution();

    std::cout << "\nGRASP:\n";
    algorithm.GRASPSolver(10,0,0).printSolution();
  } else {
    std::cout << "Error opening file\n";
    return -1;
  }
  return 0;
}
