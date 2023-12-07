#include "model.h"

void Load(std::vector<std::vector<double>> &matrix, const std::string &path) {
  std::ifstream in(path);
  std::string temp;
  double x;
  int c = 0;

  while (getline(in, temp)) {
    std::istringstream ss(temp);
    while (ss >> x) {
      matrix[c].emplace_back(x);
    }
    c++;
  }
}

int main() {
  std::vector<std::vector<double>> one;
  one.resize(5);
  std::vector<std::vector<double>> two;
  two.resize(5);
  // std::string file = "/Users/morfinov/Parallels/src/Model/test.txt";
  // std::string file_2 = "/Users/morfinov/Parallels/src/Model/test2.txt";
  std::string file = "/Users/morfinov/Parallels/src/Model/5x5.txt";
  std::string file_2 = "/Users/morfinov/Parallels/src/Model/5x5_2.txt";
  // std::string file = "/Users/morfinov/Parallels/src/Model/file_500.txt";
  // std::string file_2 = "/Users/morfinov/Parallels/src/Model/file_500.txt";
  // std::string file = "/Users/morfinov/Parallels/src/Model/1000x1000.txt";
  // std::string file_2 = "/Users/morfinov/Parallels/src/Model/1000x1000.txt";
  // std::string file = "/Users/morfinov/Parallels/src/Model/3x3.txt";
  // std::string file_2 = "/Users/morfinov/Parallels/src/Model/3x3_2.txt";
  Load(one, file);
  Load(two, file_2);
  std::cout << "LOAD DONE" << std::endl;
  // SEQUENCE ALGO!
  Winograd st(one, two);
  // int c = 0;
  auto t1 = std::chrono::high_resolution_clock::now();
  // while (c++ < 1) {
  auto res = st.Standart();
  // }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total_1 =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "seq time - " << total_1 << std::endl;

  for (auto &r : res) {
    for (auto &print : r) {
      std::cout << print << " ";
    }
    std::cout << std::endl;
  }

  Winograd par(one, two);
  // int z = 0;
  auto t5 = std::chrono::high_resolution_clock::now();
  // while (z++ < 1) {
  auto res_par = par.Parallel();
  // }
  auto t6 = std::chrono::high_resolution_clock::now();
  auto total_3 =
      std::chrono::duration_cast<std::chrono::seconds>(t6 - t5).count();
  std::cout << "parallel time - " << total_3 << std::endl;

  for (auto &r : res_par) {
    for (auto &print : r) {
      std::cout << print << " ";
    }
    std::cout << std::endl;
  }

  Winograd pipe(one, two);
  // int t = 0;
  auto t3 = std::chrono::high_resolution_clock::now();
  // while (t++ < 1) {
  auto res_pipe = pipe.Pipeline();
  // }
  auto t4 = std::chrono::high_resolution_clock::now();
  auto total_2 =
      std::chrono::duration_cast<std::chrono::seconds>(t4 - t3).count();
  std::cout << "pipeline time - " << total_2 << std::endl;

  for (auto &r : res_pipe) {
    for (auto &print : r) {
      std::cout << print << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}