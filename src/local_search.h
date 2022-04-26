/**
 * @file local_search.h
 * @author Airam Rafael Luque León (alu0101335148@ull.edu.es)
 * @brief File that contains the declaration of the class LocalSearch.
 * @version 0.1
 * @date 2022-04-25
 */

#ifndef ___LOCAL_SEARCH___
#define ___LOCAL_SEARCH___

#include "solution.h"
#include "problem.h"

/** @brief Class that implements the local search methods */
class LocalSearch {
  private:
    Problem* problem_;

  public:
    LocalSearch();
    ~LocalSearch();

    void setProblem(Problem* problem);
    Solution run(Solution initial_solution);

    // Swap intraroute
    Solution swapIntraRoute(Solution initial_solution);
    void intraRouteSwapProcedure(Route& route);
    int swapCost(int first_index, int second_index, Route route);

    // Reinsertion intraroute
    Solution reinsertionIntraRoute(Solution initial_solution);
    void intraRouteReinsertionProcedure(Route& route);
    int reinsertionCost(int first_index, int second_index, Route route);

    // Swap Interroute
    Solution swapInterRoute(Solution initial_solution);
    void interRouteSwapProcedure(Route& first_route, Route& second_route);
    std::pair<int,int> swapCost(int first_index, int second_index, 
                                Route first_route, Route second_route);

    // Reinsertion Interroute
    Solution reinsertionInterRoute(Solution initial_solution);
    void interRouteReinsertionProcedure(Route& first_route, Route& second_route);
    std::pair<int,int> reinsertionCost(int first_index, int second_index, 
                                       Route first_route, Route second_route);
};

#endif