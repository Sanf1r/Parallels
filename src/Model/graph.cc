#include "graph.h"

namespace s21 {

/**
 * @brief Загрузка графа из файла в формате матрицы смежности
 *
 *
 * @param filename
 * @return true
 * @return false
 */
bool Graph::LoadGraphFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (file.bad()) return false;
  std::string temp;
  std::getline(file, temp);
  std::istringstream ss(temp);

  data_.Clear();

  ss >> size_;
  if (size_ < 1) return false;

  data_.Init(size_);

  for (int i = 0; i < size_; ++i) {
    std::getline(file, temp);
    std::istringstream ss_in(temp);

    int x;
    int c = 0;
    while (ss_in >> x || !ss_in.eof()) {
      if (ss_in.fail()) return false;
      if (x != 0) {
        data_.Push(x);
      } else {
        data_.Push(INFINITY);
      }
      c++;
    }
    if (c != size_) return false;
  }
  return true;
}

/**
 * @brief Выгрузка графа в файл в формате dot (см. материалы)
 *
 * @param filename
 */
bool Graph::ExportGraphToDot(const std::string& filename) {
  if (size_ != 0) {
    bool showLabel = true;
    bool showDir = false;
    std::ofstream f(filename);
    f << "digraph G {" << std::endl;
    for (int i = 0; i < size_; ++i) {
      for (int j = i; j < size_; ++j) {
        if (data_(i, j) != 0) {
          std::string dir = "none";
          if (showDir) dir = "forward";
          if (showLabel) {
            f << i + 1 << " -> " << j + 1 << " [dir=" << dir
              << " label=" << data_(i, j) << "]" << std::endl;
          } else {
            f << i + 1 << " -> " << j + 1 << " [dir=" << dir << "]"
              << std::endl;
          }
        }
      }
    }
    f << "}" << std::endl;
    f.close();
  } else {
    return false;
  }
  return true;
}

}  // namespace s21
