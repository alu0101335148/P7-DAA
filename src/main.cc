/**
 * @file main.cc
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief Main file for the project.
 * @version 1.0.5
 * @date 2022-04-15
 */

#include "algorithm.h"

#include <ctime>


bool checkSolution (Solution solution_to_check, Problem problem) {
  Matrix distance_matrix = problem.getDistanceMatrix();
  std::vector<Route> routes = solution_to_check.getRoutes();
  int real_cost = 0;
  for (size_t i = 0; i < routes.size(); i++) {
    std::vector<int> route = routes[i].getRoute();
    for (size_t j = 0; j < route.size() - 1; j++) {
      real_cost += distance_matrix[route[j]][route[j + 1]];
    }
  }
  std::cout << real_cost << " : " << solution_to_check.getCost() << std::endl;
  return real_cost == solution_to_check.getCost();
}


/**
 * @brief main function of the problem
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

    // std::cout << "Normal Greedy:\n";
    // Solution greedy = algorithm.greedySolver();
    // if (checkSolution(greedy, problem)) {
    //   greedy.printSolution();
    // } else {
    //   greedy.printSolution();
    //   std::cout << "Solution not valid (greedy)\n";
    // }

    // std::cout << "\nConstructive:\n";
    // Solution grc = algorithm.GRC(rand());
    // if (checkSolution(grc, problem)) {
    //   grc.printSolution();
    // } else {
    //   std::cout << "Solution not valid (GRC)\n";
    // }

    // // GRASP SOLVER: max_iterations, seed, local_search, initial_node

    // std::cout << "\nGRASP IntraSwap:\n";
    // Solution g0 = algorithm.GRASPSolver(100,rand(),0);
    // if (checkSolution(g0, problem)) {
    //   g0.printSolution();
    // } else {
    //   std::cout << "Solution not valid (g0)\n";
    // }

    // std::cout << "\nGRASP InterSwap:\n";
    // Solution g1 = algorithm.GRASPSolver(100,rand(),1);
    // if (checkSolution(g1, problem)) {
    //   g1.printSolution();
    // } else {
    //   std::cout << "Solution not valid (g1)\n";
    // }

    // std::cout << "\nGRASP IntraReinsertion:\n";
    // Solution g2 = algorithm.GRASPSolver(100,rand(),2);
    // if (checkSolution(g2, problem)) {
    //   g2.printSolution();
    // } else {
    //   std::cout << "Solution not valid (g2)\n";
    // }

    // std::cout << "\nGRASP InterReinsertion:\n";
    // Solution g3 = algorithm.GRASPSolver(100,rand(),3);
    // if (checkSolution(g3, problem)) {
    //   g3.printSolution();
    // } else {
    //   std::cout << "Solution not valid (g3)\n";
    // }

    // std::cout << "\nGRASP 2-OPT:\n";
    // srand(std::time(NULL));
    // Solution g4 = algorithm.GRASPSolver(100,rand(),4);
    // if (checkSolution(g4, problem)) {
    //   g4.printSolution();
    // } else {
    //   std::cout << "Solution not valid (g4)\n";
    // }

    std::cout << "GVNS:\n";
    Solution gvns_solution = algorithm.GVNSSolver(0);
    if (checkSolution(gvns_solution, problem)) {
      gvns_solution.printSolution();
    } else {
      gvns_solution.printSolution();
      std::cout << "Solution not valid (gvns)\n";
      return -1;
    }
  } else {
    std::cout << "Error opening file\n";
    return -1;
  }
  return 0;
}
