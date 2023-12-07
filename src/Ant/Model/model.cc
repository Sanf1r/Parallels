#include "model.h"

namespace s21 {

TsmResult Model::SolveTravelingSalesmanProblem(int loops) {
  return ga_.SolveSalesmansProblem(graph_, loops);
}

TsmResult Model::SolveTravelingSalesmanProblemParallel(int loops) {
  return ga_.SolveSalesmansProblemParallel(graph_, loops);
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
