#ifndef A2_SIMPLENAVIGATOR_SRC_VIEW_VIEW_H_
#define A2_SIMPLENAVIGATOR_SRC_VIEW_VIEW_H_

#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../Controller/controller.h"

enum Choice { TSP = 1, EXIT = 0 };

namespace s21 {

class View {
 public:
  explicit View(Controller* c);
  void DisplayMenu();

  int PerformChoice();
  void StartEventLoop();

 private:
  Controller* c_;

  bool Input(int* loops);

  bool Load();

  bool LoadCheck(bool flag);

  void Bfs();

  void Dfs();

  void Dijkstra();

  void FloWa();

  void Msp();

  bool TspIntro();
  void Tsp(int loops);
  void TspParallel(int loops);
};
}  // namespace s21

#endif  //  A2_SIMPLENAVIGATOR_SRC_VIEW_VIEW_H_