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
    int threads_num = 1;
    int output_res = 0;
    DisplayMenu();

    switch ((Choice)PerformChoice()) {
      case WIN:
        WinIntro();
        if (MatrixMode() && OutputResult(&output_res) && InputLoops(&loops) &&
            InputThreads(&threads_num)) {
          if (c_->BeforeCalculation(4)) {
            WinStandart(loops);
            WinParallel(loops);
            WinPipeline(loops);
            if (output_res == 1) {
              std::cout << "Result of sequence calculations:" << std::endl;
              c_->PrintStandartRes();
              std::cout << "Result of parallel calculations:" << std::endl;
              c_->PrintParallelRes();
              std::cout << "Result of pipeline calculations:" << std::endl;
              c_->PrintPipelineRes();
            }
          } else {
            std::cout
                << "Error! The number of columns in the first matrix must be "
                   "equal to the number of rows in the second matrix!"
                << std::endl;
          }
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

bool View::InputLoops(int* loops) {
  std::cout << "Please, enter number of executions: ";
  std::cin >> *loops;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::cin || *loops < 1) {
    std::cout << "Wrong number of executions!" << std::endl;
    return false;
  }
  return true;
}

bool View::InputThreads(int* threads_num) {
  std::cout << "Please, enter number of threads for parallelism: ";
  std::cin >> *threads_num;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::cin || *threads_num < 1) {
    std::cout << "Wrong number of threads!" << std::endl;
    return false;
  }
  return true;
}

bool View::Load() {
  std::string f_path, s_path;
  std::cout << "Please, enter path to first matrix file: ";
  std::cin >> f_path;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cout << "Please, enter path to second matrix file: ";
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

bool View::Generate() {
  int f_rows, f_cols, s_rows, s_cols;
  std::cout << "Please, enter dimensions of first matrix: ";
  std::cin >> f_rows;
  std::cin >> f_cols;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::cin || (f_rows < 1 || f_cols < 1)) {
    std::cout << "Wrong dimensions of first matrix!" << std::endl;
    return false;
  }
  std::cout << "Please, enter dimensions of first matrix: ";
  std::cin >> s_rows;
  std::cin >> s_cols;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::cin || (s_rows < 1 || s_cols < 1)) {
    std::cout << "Wrong dimensions of second matrix!" << std::endl;
    return false;
  }
  c_->GenerateMatrix(f_rows, f_cols, s_rows, s_cols);
  std::cout << "Matrices is successfully generated!" << std::endl;
  return true;
}

bool View::MatrixMode() {
  std::cout << "Please, select how acquire matrices: " << std::endl;
  std::cout << "1. Load matrices from file" << std::endl;
  std::cout << "2. Sets dimensions of the matrices and generate randomly"
            << std::endl
            << std::endl;

  std::cout << "Input a menu item digit: ";
  int choice;
  std::cin >> choice;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::cin || choice == 0) {
    std::cout << "Wrong choice!" << std::endl;
    return false;
  }
  if (choice == 1)
    return Load();
  else
    return Generate();
}

bool View::OutputResult(int* output_res) {
  std::cout << "Please, select do you want output of result matrices of screen:"
            << std::endl;
  std::cout << "1. Yes" << std::endl;
  std::cout << "2. No" << std::endl << std::endl;

  std::cout << "Input a menu item digit: ";
  std::cin >> *output_res;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (!std::cin || (*output_res != 1 && *output_res != 2)) {
    std::cout << "Wrong choice!" << std::endl;
    return false;
  }
  return true;
}

void View::WinIntro() {
  std::cout << "=======================" << std::endl;
  std::cout << " MATRIX MULTIPLICATION " << std::endl;
  std::cout << "=======================" << std::endl;
}

void View::WinStandart(int loops) {
  std::cout << "Start sequence calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  c_->Standart(loops);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "Sequence time - " << total << " seconds" << std::endl;
}

void View::WinParallel(int loops) {
  std::cout << "Start parallel calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  c_->Parallel(loops);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "Parallel time - " << total << " seconds" << std::endl;
}

void View::WinPipeline(int loops) {
  std::cout << "Start pipeline calculations..." << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  c_->Pipeline(loops);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "Pipeline time - " << total << " seconds" << std::endl;
}

}  // namespace s21
