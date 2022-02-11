#ifndef LEFTICUS_TOOLS_STATIC_VIEWS_HPP
#define LEFTICUS_TOOLS_STATIC_VIEWS_HPP

#include <span>
#include <string_view>
#include <array>
#include <algorithm>

namespace lefticus::tools {

// max size object we can convert
static constexpr std::size_t oversized_size = 10 * 1024;


template<typename Value>
concept is_iterable = requires(const Value &value)
{
  value.begin();
  value.end();
};

template<typename Callable>
concept creates_iterable = requires(const Callable &callable)
{
  requires is_iterable<std::decay_t<decltype(callable())>>;
};


template<typename Callable>
concept creates_string_like = requires(const Callable &callable)
{
  requires creates_iterable<Callable>;
  typename std::decay_t<decltype(callable())>::traits_type::char_traits;
};


template<typename Value> struct oversized_array
{
  std::array<Value, oversized_size> data{};
  std::size_t size{};
  using value_type = Value;

  constexpr auto begin() const noexcept { return data.begin(); }
  constexpr auto end() const noexcept { return std::next(data.begin(), size); }
};

template<typename Data> constexpr auto to_oversized_array(const Data &str)
{
  oversized_array<Data::value_type> result;
  std::copy(str.begin(), str.end(), result.data.begin());
  result.size = str.size();
  return result;
}

consteval auto to_right_sized_array(creates_iterable auto callable)
{
  constexpr auto oversized = to_oversized_array(callable());

  using Value_Type = std::decay_t<decltype(oversized)>::value_type;
  std::array<Value_Type, oversized.size> result;
  std::copy(oversized.begin(), oversized.end(), result.begin());
  return result;
}

template<auto Data> consteval const auto &make_static() { return Data; }

consteval auto to_string_view(creates_string_like auto callable)
{
  constexpr auto &static_data = make_static<to_right_sized_array(callable)>();
  using Value_Type = std::decay_t<decltype(static_data)>::value_type;
  std::basic_string_view<Value_Type> result(static_data.begin(), static_data.end());
  return result;
}

consteval auto to_span(creates_iterable auto callable)
{
  constexpr auto &static_data = make_static<to_right_sized_array(callable)>();
  using Value_Type = std::decay_t<decltype(static_data)>::value_type;
  std::span<const Value_Type> result(static_data.begin(), static_data.end());
  return result;
}

}// namespace lefticus::tools

#endif
