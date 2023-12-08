#ifndef PARALLELS_SRC_ANT_MODEL_GRAPH_ALGORITHMS_H_
#define PARALLELS_SRC_ANT_MODEL_GRAPH_ALGORITHMS_H_

#include <algorithm>
#include <random>
#include <set>

#include "ant.h"
#include "ants_logic.h"
#include "graph.h"

namespace s21 {

class GraphAlgorithms {
 public:
  TsmResult SolveSalesmansProblem(const Graph& graph, int loops);

  TsmResult SolveSalesmansProblemParallel(const Graph& graph, int loops);
};

}  // namespace s21

#endif  //  PARALLELS_SRC_ANT_MODEL_GRAPH_ALGORITHMS_H_