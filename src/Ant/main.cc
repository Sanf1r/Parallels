#include "Controller/controller.h"
#include "View/view.h"

int main() {
  std::locale::global(std::locale("C"));
  s21::Model model;
  s21::Controller controller(&model);
  s21::View view(&controller);
  view.StartEventLoop();

  return 0;
}