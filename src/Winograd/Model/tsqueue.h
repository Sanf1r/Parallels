#ifndef PARALLELS_SRC_WINOGRAD_MODEL_TSQUEUE_H_
#define PARALLELS_SRC_WINOGRAD_MODEL_TSQUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

namespace s21 {

template <typename T>
class TSQueue {
 private:
  std::queue<T> m_queue_;

  std::mutex m_mutex_;

  std::condition_variable m_cond_;

 public:
  void push(T item) {
    std::unique_lock<std::mutex> lock(m_mutex_);

    m_queue_.push(item);

    m_cond_.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(m_mutex_);

    m_cond_.wait(lock, [this]() { return !m_queue_.empty(); });

    T item = m_queue_.front();
    m_queue_.pop();

    return item;
  }
};
}  // namespace s21

#endif  //  PARALLELS_SRC_WINOGRAD_MODEL_TSQUEUE_H_