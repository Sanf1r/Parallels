#ifndef PARALLELS_SRC_ANT_MODEL_GRAPH_H_
#define PARALLELS_SRC_ANT_MODEL_GRAPH_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

#include "adj_matrix.h"

namespace s21 {

class Graph {
 public:
  Graph() = default;

  int GetSize() const { return size_; }

  const double& operator()(int row, int columns) const {
    return data_(row, columns);
  }

  double& operator()(int row, int columns) { return data_(row, columns); }

  const std::vector<double>& GetData() const { return data_.GetData(); }

  bool LoadGraphFromFile(const std::string& path);

 private:
  int size_ = 0;
  AdjMatrix data_;
};

}  // namespace s21

#endif  //  PARALLELS_SRC_ANT_MODEL_GRAPH_H_