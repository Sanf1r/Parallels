#ifndef PARALLELS_SRC_WINOGRAD_VIEW_VIEW_H_
#define PARALLELS_SRC_WINOGRAD_VIEW_VIEW_H_

#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../Controller/controller.h"

enum Choice { WIN = 1, EXIT = 0 };

namespace s21 {

class View {
 public:
  explicit View(Controller* c);

  void StartEventLoop();

 private:
  Controller* c_;

  void DisplayMenu();
  int PerformChoice();

  bool InputLoops(int* loops);
  bool InputThreads(int* threads_num);
  bool OutputResult(int* output_res);

  bool Load();

  void WinIntro();

  bool MatrixMode();
  bool Generate();

  void WinStandart(int loops);
  void WinParallel(int loops);
  void WinPipeline(int loops);
};
}  // namespace s21

#endif  //  PARALLELS_SRC_WINOGRAD_VIEW_VIEW_H_
