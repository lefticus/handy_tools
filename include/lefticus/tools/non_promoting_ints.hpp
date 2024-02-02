#ifndef LEFTICUS_TOOLS_NON_PROMOTING_INTS_HPP
#define LEFTICUS_TOOLS_NON_PROMOTING_INTS_HPP

#include <compare>
#include <concepts>
#include <cstdint>

#ifdef __INTELLISENSE__
  #pragma diag_suppress 349
#endif

namespace lefticus::tools {
template<std::integral Type> struct int_np
{
  using value_type = Type;

private:
  value_type value;

public:
  // if it's the proper type, make it easy to convert
  // cppcheck-suppress noExplicitConstructor
  constexpr int_np(value_type value_) noexcept : value{ value_ } {}

  // you must use `from` if you want to create an int_np with a different input
  constexpr int_np(const auto &) = delete;

  constexpr explicit operator value_type() const noexcept { return value; }

  [[nodiscard]] constexpr value_type get() const noexcept { return value; }

  [[nodiscard]] static constexpr int_np from(const std::integral auto value)
  {
    return int_np{ static_cast<value_type>(value) };
  }

  // all of these operations are limited to exact type matches
  // so there is no question at all about what promotion should happen
  [[nodiscard]] friend constexpr int_np operator|(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value | rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator&(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value & rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator^(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value ^ rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator*(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value * rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator+(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value + rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator-(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value - rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator%(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value % rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator/(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value / rhs.value);
  }

  constexpr int_np operator~() const noexcept { return from(~value); }

  constexpr int_np &operator++() &noexcept
  {
    ++value;
    return *this;
  }

  // you shouldn't use post-increment unless you plan to use the
  // result. So I made it [[nodiscard]] - Jason
  [[nodiscard]] constexpr int_np operator++(int) &noexcept { return from(value++); }

  constexpr int_np &operator--() &noexcept
  {
    --value;
    return *this;
  }

  // you shouldn't use post-increment unless you plan to use the
  // result. So I made it [[nodiscard]] - Jason
  [[nodiscard]] constexpr int_np operator--(int) &noexcept { return from(value--); }

  // bitwise operations must be a specific match and
  // we'll get implicit conversion into the operator if it's safe
  constexpr int_np &operator&=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value & rhs.value);
    return *this;
  }
  constexpr int_np &operator|=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value | rhs.value);
    return *this;
  }
  constexpr int_np &operator^=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value ^ rhs.value);
    return *this;
  }

  // operations that are safe to perform with any integral value.
  // assignment operations cannot have a reasonable promotion no matter what
  // and shift operations are operations that are specifically performend *on*
  // the LHS
  [[nodiscard]] friend constexpr int_np operator<<(const int_np lhs, const std::integral auto rhs) noexcept
  {
    return from(lhs.value << rhs);
  }
  [[nodiscard]] friend constexpr int_np operator>>(const int_np lhs, const std::integral auto rhs) noexcept
  {
    return from(lhs.value >> rhs);
  }

  constexpr int_np &operator<<=(const std::integral auto rhs) &noexcept
  {
    value = static_cast<value_type>(value << rhs);
    return *this;
  }

  constexpr int_np &operator>>=(const std::integral auto rhs) &noexcept
  {
    value = static_cast<value_type>(value >> rhs);
    return *this;
  }

  constexpr int_np &operator+=(const std::integral auto rhs) &noexcept
  {
    value = static_cast<value_type>(value + rhs);
    return *this;
  }

  constexpr int_np &operator-=(const std::integral auto rhs) &noexcept
  {
    value = static_cast<value_type>(value - rhs);
    return *this;
  }

  constexpr int_np &operator/=(const std::integral auto rhs) &noexcept
  {
    value = static_cast<value_type>(value / rhs);
    return *this;
  }

  constexpr int_np &operator%=(const std::integral auto rhs) &noexcept
  {
    value = static_cast<value_type>(value / rhs);
    return *this;
  }

  // np versions

  [[nodiscard]] friend constexpr int_np operator<<(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value << rhs.value);
  }
  [[nodiscard]] friend constexpr int_np operator>>(const int_np lhs, const int_np rhs) noexcept
  {
    return from(lhs.value >> rhs.value);
  }

  constexpr int_np &operator<<=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value << rhs.value);
    return *this;
  }

  constexpr int_np &operator>>=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value >> rhs.value);
    return *this;
  }

  constexpr int_np &operator+=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value + rhs.value);
    return *this;
  }

  constexpr int_np &operator-=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value - rhs.value);
    return *this;
  }

  constexpr int_np &operator/=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value / rhs.value);
    return *this;
  }

  constexpr int_np &operator%=(const int_np rhs) &noexcept
  {
    value = static_cast<value_type>(value / rhs.value);
    return *this;
  }

  friend constexpr auto operator<=>(const int_np &, const int_np &) = default;
};

using uint_np8_t = int_np<std::uint8_t>;
using uint_np16_t = int_np<std::uint16_t>;
using uint_np32_t = int_np<std::uint32_t>;
using uint_np64_t = int_np<std::uint64_t>;

using int_np8_t = int_np<std::int8_t>;
using int_np16_t = int_np<std::int16_t>;
using int_np32_t = int_np<std::int32_t>;
using int_np64_t = int_np<std::int64_t>;

namespace literals {

  consteval auto operator"" _npu8(unsigned long long val) { return uint_np8_t::from(val); }

  consteval auto operator"" _npu16(unsigned long long val) { return uint_np16_t::from(val); }

  consteval auto operator"" _npu32(unsigned long long val) { return uint_np32_t::from(val); }

  consteval auto operator"" _npu64(unsigned long long val) { return uint_np64_t::from(val); }


  consteval auto operator"" _np8(unsigned long long val) { return int_np8_t::from(val); }

  consteval auto operator"" _np16(unsigned long long val) { return int_np16_t::from(val); }

  consteval auto operator"" _np32(unsigned long long val) { return int_np32_t::from(val); }

  consteval auto operator"" _np64(unsigned long long val) { return int_np64_t::from(val); }
}// namespace literals

}// namespace lefticus::tools
#endif
