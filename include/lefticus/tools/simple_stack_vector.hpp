/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
*/


#ifndef LEFTICUS_TOOLS_SIMPLE_STACK_VECTOR_HPP
#define LEFTICUS_TOOLS_SIMPLE_STACK_VECTOR_HPP

#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace lefticus::tools {


// changes from std::vector
//  * capacity if fixed at compile-time
//  * it never allocates
//  * items must be default constructible
//  * items are never destroyed until the entire stack_vector
//    is destroyed.
//  * iterators are never invalidated
//  * capacity() and max_size() are now static functions
//  * should be fully C++17 usable within constexpr
template<typename Contained, std::size_t Capacity> struct simple_stack_vector
{
  using value_type = Contained;
  using data_type = std::array<value_type, Capacity>;
  using size_type = typename data_type::size_type;
  using difference_type = typename data_type::difference_type;
  using reference = value_type &;
  using const_reference = const value_type &;

  static_assert(std::is_default_constructible_v<Contained>);

  using iterator = typename data_type::iterator;
  using const_iterator = typename data_type::const_iterator;
  using reverse_iterator = typename data_type::reverse_iterator;
  using const_reverse_iterator = typename data_type::const_reverse_iterator;

  constexpr simple_stack_vector() = default;
  constexpr explicit simple_stack_vector(std::initializer_list<value_type> values)
  {
    for (const auto &value : values) { push_back(value); }
  }

  template<typename OtherContained, std::size_t OtherSize>
  constexpr explicit simple_stack_vector(const simple_stack_vector<OtherContained, OtherSize> &other)
  {
    for (const auto &value : other) { push_back(Contained{ value }); }
  }

  template<typename Type> constexpr explicit simple_stack_vector(const std::vector<Type> &values)
  {
    for (const auto &value : values) { push_back(Contained{ value }); }
  }

  template<typename Itr> constexpr simple_stack_vector(Itr begin, Itr end)
  {
    while (begin != end) {
      push_back(*begin);
      ++begin;
    }
  }

  [[nodiscard]] constexpr iterator begin() noexcept { return data_.begin(); }

  [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_.cbegin(); }
  [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return data_.cbegin(); }

  [[nodiscard]] constexpr iterator end() noexcept
  {
    return std::next(data_.begin(), static_cast<difference_type>(size_));
  }

  [[nodiscard]] constexpr const_iterator end() const noexcept
  {
    return std::next(data_.cbegin(), static_cast<difference_type>(size_));
  }

  [[nodiscard]] constexpr value_type &front() noexcept { return data_.front(); }
  [[nodiscard]] constexpr const value_type &front() const noexcept { return data_.front(); }
  [[nodiscard]] constexpr value_type &back() noexcept { return data_.back(); }
  [[nodiscard]] constexpr const value_type &back() const noexcept { return data_.back(); }

  [[nodiscard]] constexpr const_iterator cend() const noexcept { return end(); }

  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
  {
    return std::next(data_.rbegin(), static_cast<difference_type>(Capacity - size_));
  }

  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
  {
    return std::next(data_.crbegin(), static_cast<difference_type>(Capacity - size_));
  }
  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

  [[nodiscard]] constexpr reverse_iterator rend() noexcept { return data_.rend(); }

  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return data_.crend(); }

  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return data_.crend(); }

  template<typename Value> constexpr value_type &push_back(Value &&value)
  {
    if (size_ == Capacity) { throw std::length_error("push_back would exceed static capacity"); }
    data_[size_] = std::forward<Value>(value);
    return data_[size_++];
  }

  template<typename... Param> constexpr value_type &emplace_back(Param &&...param)
  {
    if (size_ == Capacity) { throw std::length_error("emplace_back would exceed static capacity"); }
    data_[size_] = value_type{ std::forward<Param>(param)... };
    return data_[size_++];
  }

  [[nodiscard]] constexpr value_type &operator[](const std::size_t idx) noexcept { return data_[idx]; }

  [[nodiscard]] constexpr const value_type &operator[](const std::size_t idx) const noexcept { return data_[idx]; }

  [[nodiscard]] constexpr value_type &at(const std::size_t idx)
  {
    if (idx >= size_) { throw std::out_of_range("index past end of stack_vector"); }
    return data_[idx];
  }

  [[nodiscard]] constexpr const value_type &at(const std::size_t idx) const
  {
    if (idx >= size_) { throw std::out_of_range("index past end of stack_vector"); }
    return data_[idx];
  }

  // resets the size to 0, but does not destroy any existing objects
  constexpr void clear() { size_ = 0; }


  // cppcheck-suppress functionStatic
  constexpr void reserve(size_type new_capacity)
  {
    if (new_capacity > Capacity) { throw std::length_error("new capacity would exceed max_size for stack_vector"); }
  }

  // cppcheck-suppress functionStatic
  [[nodiscard]] constexpr static size_type capacity() noexcept { return Capacity; }

  // cppcheck-suppress functionStatic
  [[nodiscard]] constexpr static size_type max_size() noexcept { return Capacity; }

  [[nodiscard]] constexpr size_type size() const noexcept { return size_; }


  constexpr void resize(const size_type new_size)
  {
    if (new_size <= size_) {
      size_ = new_size;
    } else {
      if (new_size > Capacity) {
        throw std::length_error("resize would exceed static capacity");
      } else {
        auto old_end = end();
        size_ = new_size;
        auto new_end = end();
        while (old_end != new_end) {
          *old_end = data_type{};
          ++old_end;
        }
      }
    }
  }

  constexpr void pop_back() noexcept { --size_; }

  // cppcheck-suppress functionStatic
  constexpr void shrink_to_fit() noexcept
  {
    // nothing to do here
  }


private:
  // default initializing to make it more C++17 friendly
  data_type data_{};
  size_type size_{};
};


template<typename Contained, std::size_t LHSSize, std::size_t RHSSize>
[[nodiscard]] constexpr bool operator==(const simple_stack_vector<Contained, LHSSize> &lhs,
  const simple_stack_vector<Contained, RHSSize> &rhs)
{
  if (lhs.size() == rhs.size()) {
    for (std::size_t idx = 0; idx < lhs.size(); ++idx) {
      if (lhs[idx] != rhs[idx]) { return false; }
    }
    return true;
  }

  return false;
}

}// namespace lefticus::tools


#endif
