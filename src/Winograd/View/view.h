#ifndef SRC_VIEW_VIEW_H_
#define SRC_VIEW_VIEW_H_

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
  void DisplayMenu();

  int PerformChoice();
  void StartEventLoop();

 private:
  Controller* c_;

  bool Input(int* loops);

  bool Load();

  bool LoadCheck(bool flag);

  bool WinIntro();
};
}  // namespace s21

#endif  //  SRC_VIEW_VIEW_H_
