#include "view.h"

namespace s21 {

View::View(Controller* c) : c_(c) {}

void View::DisplayMenu() {
  std::cout << "=========" << std::endl;
  std::cout << " M E N U " << std::endl;
  std::cout << "=========" << std::endl;
  std::cout << "1. Winograd algorithm of matrix multiplication" << std::endl;
  std::cout << "0. Quit" << std::endl << std::endl;
}

int View::PerformChoice() {
  int choice;
  std::cout << "Input a menu item digit: ";
  std::cin >> choice;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return choice;
}

void View::StartEventLoop() {
  while (true) {
    int loops = 0;
    DisplayMenu();

    switch ((Choice)PerformChoice()) {
      case WIN:
        if (WinIntro() && Input(&loops)) {
          c_->BeforeCalculation(4);
          WinStandart(loops);
          c_->PrintStandartRes();
          WinParallel(loops);
          c_->PrintParallelRes();
          WinPipeline(loops);
          c_->PrintPipelineRes();
        }
        break;

      case EXIT:
        exit(1);

      default:
        std::cout << "Wrong menu item number!" << std::endl;
        continue;
    }
  }
}

bool View::Input(int* loops) {
  std::cout << "Please enter number of executions: " << std::endl;
  std::cin >> *loops;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::cin || *loops < 1) {
    std::cout << "Wrong number of executions!" << std::endl;
    return false;
  }
  return true;
}

bool View::Load() {
  std::string f_path, s_path;
  std::cout << "Please, enter path to first matrix file: " << std::endl;
  std::cin >> f_path;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cout << "Please, enter path to second matrix file: " << std::endl;
  std::cin >> s_path;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (c_->LoadMatrix(f_path, s_path)) {
    std::cout << "Matrices is successfully loaded!" << std::endl;
    return true;
  } else {
    std::cout << "Something wrong with a matrices!" << std::endl;
    return false;
  }
}

bool View::WinIntro() {
  std::cout << "=======================" << std::endl;
  std::cout << " MATRIX MULTIPLICATION " << std::endl;
  std::cout << "=======================" << std::endl;
  return Load();
}

void View::WinStandart(int loops) {
  std::cout << "Start sequence calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  c_->Standart(loops);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

  // std::cout << "Travelling salesman problem solved!" << std::endl;
  // std::cout << "Path - ";
  // for (int i = 0; i < (int)tsm_res.vertices.size(); ++i) {
  //   std::cout << tsm_res.vertices[i] + 1 << " ";
  // }
  // std::cout << std::endl;
  // std::cout << "Total distance - ";
  // std::cout << tsm_res.distance << std::endl;
  std::cout << "Time to solve - " << total << " seconds" << std::endl;
}

void View::WinParallel(int loops) {
  std::cout << "Start parallel calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  c_->Parallel(loops);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

  // std::cout << "Travelling salesman problem solved!" << std::endl;
  // std::cout << "Path - ";
  // for (int i = 0; i < (int)tsm_res.vertices.size(); ++i) {
  //   std::cout << tsm_res.vertices[i] + 1 << " ";
  // }
  // std::cout << std::endl;
  // std::cout << "Total distance - ";
  // std::cout << tsm_res.distance << std::endl;
  std::cout << "Time to solve - " << total << " seconds" << std::endl;
}

void View::WinPipeline(int loops) {
  std::cout << "Start pipeline calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  c_->Pipeline(loops);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

  // std::cout << "Travelling salesman problem solved!" << std::endl;
  // std::cout << "Path - ";
  // for (int i = 0; i < (int)tsm_res.vertices.size(); ++i) {
  //   std::cout << tsm_res.vertices[i] + 1 << " ";
  // }
  // std::cout << std::endl;
  // std::cout << "Total distance - ";
  // std::cout << tsm_res.distance << std::endl;
  std::cout << "Time to solve - " << total << " seconds" << std::endl;
}

}  // namespace s21
