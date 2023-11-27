#ifndef A2_SIMPLENAVIGATOR_SRC_MODEL_S21_STACK_H_
#define A2_SIMPLENAVIGATOR_SRC_MODEL_S21_STACK_H_

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace s21 {

template <class T>
class stack {
 public:
  // member types
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  // stack Member functions
  /**
   * @brief Конструктор создает пустой указатель с типом Node
   */
  stack() noexcept
      : back_(nullptr), root_(nullptr), size_(0) {}  // По умолчанию стек пуст

  /**
   * @brief Конструктор, где принимается список элементов и кладется в
   * контейнер.
   * @param items Список элементов на вход.
   */
  explicit stack(std::initializer_list<value_type> const& items)
      : back_(nullptr), root_(nullptr), size_(0) {
    for (auto i : items) {
      this->push(i);
    }
  }

  /**
   * @brief Конструктор копирования из одного контейнера в ново создоваемый
   * контейнер.
   * @param s Указатель на контейнер с которого необходимо скопировать.
   */
  stack(const stack& s) : back_(nullptr), root_(nullptr), size_(0) {
    Node* newnode = s.back_;
    while (newnode != nullptr) {
      push(newnode->data_);
      newnode = newnode->next_;
    }
  }

  /**
   * @brief Конструктор перемещения. Посути просто меняем указатели. Сами данные
   * остаются на месте.
   * @param s Указатель на контейнер с которого необходимо переместить данные в
   * новый контейнер.
   */
  stack(stack&& s) noexcept : back_(s.back_), root_(s.root_), size_(s.size_) {
    s.back_ = s.root_ = nullptr;
    s.size_ = 0;
  }

  /**
   * Деструктор
   */
  virtual ~stack() { delete_data(); }

  /**
   * @brief Перегруженный оператор переноса. Просто меняем указаетли. Старые
   * указатели зануляем, т.е. затираем связь с данными.
   * @param s объект стэка с котрого необходимо перенести данные.
   * @return Указатель на новый контейнер.
   */
  stack& operator=(stack&& s) {
    if (root_ != nullptr) {
      delete_data();
    }
    root_ = s.root_;
    back_ = s.back_;
    size_ = s.size_;
    s.root_ = s.back_ = nullptr;
    s.size_ = 0;
    return *this;
  }

  /**
   * @brief Перегруженный оператор копирования.
   * @param s Контейнер с которого копируем
   * @return Указатель на скопированный контейнер (ново созданный)
   */
  stack& operator=(const stack& s) {
    if (this->root_ == s.root_) return *this;
    if (root_ != nullptr) {
      delete_data();
    }
    Node* buff = s.back_;
    while (buff != nullptr) {
      push(buff->data_);
      buff = buff->next_;
    }
    size_ = s.size_;
    return *this;
  }

  /**
   * @brief Stack element access. Показывает последний элемент, который был
   * положен в контейнер.
   * @return Указатель на последний элемент контейнера.
   */
  reference top() noexcept { return root_->data_; }

  /**
   * @brief Stack element access. Показывает последний элемент, который был
   * положен в контейнер.
   * @return Указатель на последний константный элемент контейнера.
   */
  const_reference top() const noexcept { return root_->data_; }

  /**
   * @brief Проверяет есть ли что-то в контейнере.
   * @return True - пустой. False - что-то есть.
   */
  [[nodiscard]] bool empty() const noexcept { return root_ == nullptr; }

  /**
   * @brief Размер контейнера.
   * @return Количество элементов в контейнере.
   */
  [[nodiscard]] size_type size() const noexcept { return size_; }

  /**
   * @brief Ввод нового значения в контейнер
   * @param value Ссылка значения, который будет положен в контейнер
   */
  void push(const_reference value) {
    size_++;
    Node* newnode = new Node;
    newnode->data_ = value;
    newnode->next_ = nullptr;
    if (back_ != nullptr) {
      root_->next_ = newnode;
      newnode->prev_ = root_;
    } else {
      back_ = newnode;
      newnode->prev_ = nullptr;
    }
    root_ = newnode;
  }

  /**
   * @breif Удаление последне добавленного элемента в контейнере.
   */
  virtual void pop() {
    if (empty()) {
      throw std::length_error("stack is empty");
    }
    size_--;
    Node* delnode = root_;
    root_ = root_->prev_;
    delete delnode;
    if (root_ == nullptr) {
      back_ = nullptr;
    } else {
      root_->next_ = nullptr;
    }
  }

  /**
   * Обмен данными между двумя контейнерами.
   * @param other ссылка на Контейнер.
   */
  void swap(stack& other) noexcept {
    stack buff = other;
    other = *this;
    *this = buff;
  }

  /**
   * Когда хотим положить объект класса в контейнер и чтобы не вызывался
   * конструктор перемещения, т.е. конструктор не будет взываеться при вставке
   * объекта класса в контейнер с типом данного класса.
   * @tparam Args
   * @param args
   */
  template <class... Args>
  void emplace_front(Args&&... args) {
    value_type buf(args...);
    this->push(buf);
  }

 protected:
  struct Node {
    T data_;
    Node* prev_;  // Указатель на предыдущую структуру стека
    Node* next_;  // Указатель на следующую структуру стека
  };

  void delete_data() {
    while (back_ != nullptr) {
      this->pop();
    }
  }

  Node* back_;  // начало стека
  Node* root_;  // вершина стека
  size_t size_;
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_SRC_MODEL_S21_STACK_H_
