#include "model.h"

namespace s21 {

TsmResult Model::SolveTravelingSalesmanProblem() {
  return ga_.SolveSalesmansProblem(graph_);
}

TsmResult Model::SolveTravelingSalesmanProblemParallel() {
  return ga_.SolveSalesmansProblemParallel(graph_);
}

bool Model::LoadGraphFromFile(const std::string& path) {
  return graph_.LoadGraphFromFile(path);
}

bool Model::ExportGraphToDot(const std::string& path) {
  return graph_.ExportGraphToDot(path);
}

bool Model::UndirectedCheck() { return ga_.UndirectedCheck(graph_); }

int Model::GetSize() { return graph_.GetSize(); }

}  // namespace s21
