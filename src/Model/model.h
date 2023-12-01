#ifndef SRC_MODEL_MODEL_H_
#define SRC_MODEL_MODEL_H_

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <vector>

#include "adj_matrix.h"
#include "ant.h"
#include "ants_logic.h"
#include "graph_algorithms.h"

namespace s21 {

class Model {
 public:
  TsmResult SolveTravelingSalesmanProblem(int loops);

  TsmResult SolveTravelingSalesmanProblemParallel(int loops);

  bool LoadGraphFromFile(const std::string& path);

  bool ExportGraphToDot(const std::string& path);

  bool UndirectedCheck();

  int GetSize();

 private:
  Graph graph_;
  GraphAlgorithms ga_;
};
}  // namespace s21

#endif  //  SRC_MODEL_MODEL_H_
