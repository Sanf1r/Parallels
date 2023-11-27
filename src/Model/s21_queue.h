#ifndef A2_SIMPLENAVIGATOR_SRC_MODEL_S21_QUEUE_H_
#define A2_SIMPLENAVIGATOR_SRC_MODEL_S21_QUEUE_H_

#include "s21_stack.h"

namespace s21 {

template <class T>
class queue : public stack<T> {
 private:
  using stack<T>::emplace_front;

 public:
  // member types
  using value_type = typename stack<T>::value_type;
  using const_reference = typename stack<T>::const_reference;
  using Node = typename stack<T>::Node;

  // member functions
  queue() noexcept : stack<T>() {}
  explicit queue(std::initializer_list<value_type> const &items)
      : stack<T>(items) {}
  queue(const queue &q) : stack<T>(q) {}
  queue(queue &&q) noexcept : stack<T>(q) {}

  queue &operator=(queue &&s) {
    if (this->root_ != nullptr) {
      stack<T>::delete_data();
    }
    this->root_ = s.root_;
    this->back_ = s.back_;
    this->size_ = s.size_;
    s.root_ = s.back_ = nullptr;
    s.size_ = 0;
    return *this;
  }

  queue &operator=(const queue &q) {
    if (this->root_ == q.root_) return *this;
    if (this->root_ != nullptr) {
      stack<T>::delete_data();
    }
    Node *buff = q.root_;
    while (buff != nullptr) {
      stack<T>::push(buff->data_);
      buff = buff->prev_;
    }
    return *this;
  }

  // queue element access
  const_reference front() noexcept { return stack<T>::top(); }
  const_reference back() noexcept { return stack<T>::back_->data_; }

  /**
   * @breif Queue modifiers.
   * Удаление самого первого элемента, который был добавлен в контейнер.
   */
  void pop() override {
    if (stack<T>::empty()) {
      throw std::length_error("stack is empty");
    }
    this->size_--;
    Node *delnode = this->back_;
    this->back_ = this->back_->next_;
    delete delnode;
    if (this->back_ == nullptr) {
      this->root_ = nullptr;
    } else {
      this->back_->prev_ = nullptr;
    }
  }

  template <class... Args>
  void emplace_back(Args &&...args) {
    stack<T>::emplace_front(std::forward<Args>(args)...);
  }
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_SRC_MODEL_S21_QUEUE_H_
