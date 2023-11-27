#include "view.h"

namespace s21 {

View::View(Controller* c) : c_(c) {}

void View::DisplayMenu() {
  std::cout << "=========" << std::endl;
  std::cout << " M E N U " << std::endl;
  std::cout << "=========" << std::endl;
  std::cout << "1. Solving the traveling salesman problem" << std::endl;
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
    DisplayMenu();

    switch ((Choice)PerformChoice()) {
      case TSP:
        if (TspIntro()) {
          Tsp();
          TspParallel();
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

bool View::Load() {
  std::string path;
  std::cout << "Please, enter path to graph file: " << std::endl;
  std::cin >> path;
  if (c_->LoadGraphFromFile(path)) {
    std::cout << "Graph is successfully loaded!" << std::endl;
    std::cout << "Start calculations..." << std::endl;
    return true;
  } else {
    std::cout << "Something wrong with a file path!" << std::endl;
    return false;
  }
}

bool View::TspIntro() {
  std::cout << "=============================" << std::endl;
  std::cout << " TRAVELLING SALESMAN PROBLEM " << std::endl;
  std::cout << "=============================" << std::endl;
  return Load();
}

void View::Tsp() {
  TsmResult tsm_res;
  auto t1 = std::chrono::high_resolution_clock::now();
  tsm_res = std::move(c_->SolveTravelingSalesmanProblem());
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  if (tsm_res.distance != INFINITY) {
    std::cout << "Travelling salesman problem solved!" << std::endl;
    std::cout << "Path - ";
    for (int i = 0; i < (int)tsm_res.vertices.size(); ++i) {
      std::cout << tsm_res.vertices[i] + 1 << " ";
    }
    std::cout << std::endl;
    std::cout << "Total distance - ";
    std::cout << tsm_res.distance << std::endl;
    std::cout << "Time to solve - " << total << " seconds" << std::endl;
  } else {
    std::cout << "Travelling salesman problem cannot be solved for "
                 "this graph!"
              << std::endl;
  }
}

void View::TspParallel() {
  TsmResult tsm_res;
  auto t1 = std::chrono::high_resolution_clock::now();
  tsm_res = std::move(c_->SolveTravelingSalesmanProblemParallel());
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  if (tsm_res.distance != INFINITY) {
    std::cout << "Travelling salesman problem solved!" << std::endl;
    std::cout << "Path - ";
    for (int i = 0; i < (int)tsm_res.vertices.size(); ++i) {
      std::cout << tsm_res.vertices[i] + 1 << " ";
    }
    std::cout << std::endl;
    std::cout << "Total distance - ";
    std::cout << tsm_res.distance << std::endl;
    std::cout << "Time to solve - " << total << " seconds" << std::endl;
  } else {
    std::cout << "Travelling salesman problem cannot be solved for "
                 "this graph!"
              << std::endl;
  }
}

}  // namespace s21
