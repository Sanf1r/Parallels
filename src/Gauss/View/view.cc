#include "view.h"

namespace s21 {

View::View(Controller* c) : c_(c) {}

void View::DisplayMenu() {
  std::cout << std::endl << "=========" << std::endl;
  std::cout << " M E N U " << std::endl;
  std::cout << "=========" << std::endl;
  std::cout << "1. Solving systems of linear equations" << std::endl;
  std::cout << "2. Show result" << std::endl;
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
      case SSLE:
        if (SsleIntro() && Input(&loops)) {
          Ssle(loops);
          SsleParallel(loops);
        }
        break;

      case RES:
        GetResult();
        SsleGetResult();
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
  std::string path;
  std::cout << "Please, enter path to graph file: " << std::endl;
  std::cin >> path;
  if (c_->LoadGraphFromFile(path)) {
    std::cout << "Matrix is successfully loaded!" << std::endl;
    return true;
  } else {
    std::cout << "Something wrong with a file path!" << std::endl;
    return false;
  }
}

bool View::SsleIntro() {
  std::cout << "=============================" << std::endl;
  std::cout << " SYSTEMS OF LINEAR EQUATIONS " << std::endl;
  std::cout << "=============================" << std::endl;
  return Load();
}

void View::Ssle(int loops) {
  int tsm_res;
  std::cout << std::endl << "Start sequence calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  tsm_res = std::move(c_->SolvingSystemsLinearEquations(loops));
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  if (tsm_res) {
    std::cout << "Systems of linear equations solved!" << std::endl;
    std::cout << std::endl;
    std::cout << "Time to solve - " << total << " seconds" << std::endl;
  } else {
    std::cout << "Systems of linear equations cannot be solved for this matrix!"
              << std::endl;
  }
}

void View::SsleParallel(int loops) {
  int tsm_res;
  std::cout << std::endl << "Start parallel calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  tsm_res = std::move(c_->SolvingSystemsLinearEquationsParallels(loops));
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  if (tsm_res) {
    std::cout << "Systems of linear equations solved!" << std::endl;
    std::cout << std::endl;
    std::cout << "Time to solve - " << total << " seconds" << std::endl;
  } else {
    std::cout << "Systems of linear equations cannot be solved for this matrix!"
              << std::endl;
  }
}

void View::GetResult() {
  if (c_->GetResultCorrect()) {
    std::vector<double> tsm_res;
    tsm_res = std::move(c_->GetResult());
    std::cout << std::endl << "Result:" << std::endl;
    for (int i = 0; i < (int)tsm_res.size(); ++i) {
      std::cout << "x" << i + 1 << " = " << tsm_res[i] << " " << std::endl;
    }

  } else {
    std::cout << "Systems of linear equations cannot be solved for this matrix!"
              << std::endl;
  }
}

void View::SsleGetResult() {
  if (c_->GetResultCorrect()) {
    std::vector<double> tsm_res;
    tsm_res = std::move(c_->GetResultParallels());
    std::cout << std::endl << "Result for parallels:" << std::endl;
    for (int i = 0; i < (int)tsm_res.size(); ++i) {
      std::cout << "x" << i + 1 << " = " << tsm_res[i] << " " << std::endl;
    }

  } else {
    std::cout << "Systems of linear equations cannot be solved for this matrix!"
              << std::endl;
  }
}

}  // namespace s21
