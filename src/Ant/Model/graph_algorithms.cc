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
TsmResult GraphAlgorithms::SolveSalesmansProblem(const Graph &graph,
                                                 int loops) {
  AntsLogic al_(graph);
  return al_.SolveSalesmansProblem(loops);
}

TsmResult GraphAlgorithms::SolveSalesmansProblemParallel(const Graph &graph,
                                                         int loops) {
  AntsLogic al_(graph);
  return al_.SolveSalesmansProblemParallel(loops);
}

}  // namespace s21
