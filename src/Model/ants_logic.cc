#include "ants_logic.h"

namespace s21 {

AntsLogic::AntsLogic(const Graph &graph) : graph_(graph) {
  pheromone_.InitWithNumber(graph.GetSize(), 0.2);
  graph_size_ = graph_.GetSize();
}

TsmResult AntsLogic::SolveSalesmansProblem() {
  int counter = 0;

  TsmResult path;
  path.distance = INFINITY;
  CreateAnts();

  while (counter++ != kMaxLoopsWithNoGains_) {
    AdjMatrix local_pheromone_update;
    local_pheromone_update.InitWithNumber(graph_size_, 0);

    // ant runs while he still can
    AntRun();
    // compare ant result to best
    LocalUpdate(path, &counter, local_pheromone_update);

    UpdateGlobalPheromone(local_pheromone_update);
    BrainwashAnts();
  }

  return path;
}

TsmResult AntsLogic::SolveSalesmansProblemParallel() {
  int counter = 0;

  TsmResult path;
  path.distance = INFINITY;
  CreateAntsParallel();

  while (counter++ != kMaxLoopsWithNoGains_) {
    AdjMatrix local_pheromone_update;
    local_pheromone_update.InitWithNumber(graph_size_, 0);

    // ant runs while he still can
    AntRunParallel();
    // compare ant result to best
    LocalUpdate(path, &counter, local_pheromone_update);

    UpdateGlobalPheromone(local_pheromone_update);
    BrainwashAntsParallel();
  }

  return path;
}

void AntsLogic::CreateAnts() {
  ants_.reserve(graph_size_);

  for (int i = 0; i < graph_size_; ++i) {
    ants_.emplace_back(Ant(i));
  }
}

void AntsLogic::CreateAntsParallel() {
  ants_.reserve(graph_size_);
  std::vector<std::thread> threads;

  for (int x = 0; x < 4; x++) {
    threads.emplace_back([&]() {
      int number = x;
      while (number < graph_size_) {
        ants_.emplace_back(Ant(number));
        number += 4;
      }
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }
}

void AntsLogic::AntRun() {
  for (auto &ant : ants_) {
    while (ant.GetMove()) {
      ant.AntMove(graph_, pheromone_, kAlpha_, kBeta_);
    }
  }
}

void AntsLogic::AntRunParallel() {
  std::for_each(std::execution::par, ants_.begin(), ants_.end(), [&](Ant &ant) {
    while (ant.GetMove()) {
      ant.AntMove(graph_, pheromone_, kAlpha_, kBeta_);
    }
  });
}

void AntsLogic::LocalUpdate(TsmResult &path, int *counter,
                            AdjMatrix &local_pheromone_update) {
  for (auto &ant : ants_) {
    TsmResult ant_path = ant.GetPath();
    if ((int)ant_path.vertices.size() == graph_size_ + 1) {
      if (ant.GetPath().distance < path.distance) {
        path = std::move(ant.GetPath());
        *counter = *counter;
      }
      // calculate pheromone delta for every edge
      double pheromone_delta = kQ_ / ant_path.distance;
      for (int i = 0; i < graph_size_; ++i) {
        local_pheromone_update(ant_path.vertices[i],
                               ant_path.vertices[i + 1]) += pheromone_delta;
      }
    }
  }
}

void AntsLogic::BrainwashAnts() {
  for (int i = 0; i < graph_size_; ++i) {
    ants_[i].BrainwashAnt();
  }
}

void AntsLogic::BrainwashAntsParallel() {
  std::for_each(std::execution::par, ants_.begin(), ants_.end(),
                [&](Ant &ant) { ant.BrainwashAnt(); });
}

void AntsLogic::UpdateGlobalPheromone(const AdjMatrix &lpu) {
  int size = graph_.GetSize();
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (i == j) continue;
      pheromone_(i, j) = kEvap_ * pheromone_(i, j) + lpu(i, j);
      if (pheromone_(i, j) < 0.2) pheromone_(i, j) = 0.2;
    }
  }
}

}  // namespace s21
