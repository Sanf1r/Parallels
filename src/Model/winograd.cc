#include "winograd.h"

void Load(std::vector<std::vector<double>> &matrix, const std::string &path) {
  std::ifstream in(path);
  std::string temp;
  int x;
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
  one.resize(500);
  std::vector<std::vector<double>> two;
  two.resize(500);
  std::string file = "/Users/morfinov/GHPara/src/Model/file_500.txt";
  std::string file_2 = "/Users/morfinov/GHPara/src/Model/file_500.txt";
  Load(one, file);
  Load(two, file_2);
  std::cout << "LOAD DONE" << std::endl;
  // SEQUENCE ALGO!
  Winograd st(one, two);
  int c = 0;
  auto t1 = std::chrono::high_resolution_clock::now();
  while (c++ < 100) {
    auto res = st.Standart();
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total_1 =
      std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  std::cout << "seq time - " << total_1 << std::endl;

  // for (auto &r : res) {
  //   for (auto &print : r) {
  //     std::cout << print << " ";
  //   }
  //   std::cout << std::endl;
  // }

  Winograd pipe(one, two);
  int t = 0;
  auto t3 = std::chrono::high_resolution_clock::now();
  while (t++ < 100) {
    auto res_pipe = pipe.Pipeline();
  }
  auto t4 = std::chrono::high_resolution_clock::now();
  auto total_2 =
      std::chrono::duration_cast<std::chrono::seconds>(t4 - t3).count();
  std::cout << "pipeline time - " << total_2 << std::endl;

  // for (auto &r : res_pipe) {
  //   for (auto &print : r) {
  //     std::cout << print << " ";
  //   }
  //   std::cout << std::endl;
  // }
  return 0;
}