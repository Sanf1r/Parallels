#ifndef SRC_MODEL_GRAPH_ALGORITHMS_H_
#define SRC_MODEL_GRAPH_ALGORITHMS_H_

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

  bool UndirectedCheck(const Graph& graph);

 private:
  int Random(int size);
};

}  // namespace s21

#endif  //  SRC_MODEL_GRAPH_ALGORITHMS_H_
