#include "graph_algorithms.h"

namespace s21 {

/**
 * @brief Решение задачи коммивояжера с помощью муравьиного алгоритма.
 * Находим самый выгодный (короткий) маршрут, проходящий
 * через все вершины графа хотя бы по одному разу с последующим
 * возвратом в исходную вершину. В качестве результата функция
 * возвращает структуру TsmResult.
 *
 * @param graph
 * @return TsmResult
 */
TsmResult GraphAlgorithms::SolveSalesmansProblem(const Graph& graph,
                                                 int loops) {
  AntsLogic al_(graph);
  return al_.SolveSalesmansProblem(loops);
}

TsmResult GraphAlgorithms::SolveSalesmansProblemParallel(const Graph& graph,
                                                         int loops) {
  AntsLogic al_(graph);
  return al_.SolveSalesmansProblemParallel(loops);
}

bool GraphAlgorithms::UndirectedCheck(const Graph& graph) {
  bool res = true;
  int graph_size = graph.GetSize();
  for (auto i = 0; i < graph_size; ++i) {
    for (auto j = i; j < graph_size; ++j) {
      if (graph(i, j) != graph(j, i)) {
        res = false;
      }
    }
  }
  return res;
}

int GraphAlgorithms::Random(int size) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> gen(0, size - 1);
  return gen(rng);
}

}  // namespace s21
