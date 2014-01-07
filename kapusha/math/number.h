#pragma once

namespace kapusha {
namespace math {

/// \brief Basic number interface
template <typename T>
struct number {
  T v;

#if _MSC_VER < 1800
  inline number() {}
#else
  number() = default;
#endif

  inline number(T t) : v(t) {}
  template <typename U>
  inline explicit number(U u) : v(static_cast<T>(u)) {}

  inline operator T() const { return v; }
  //inline operator T*() const { return &v; }
  inline operator T&() { return v; }

  /*
  inline number operator+(const number &other) const { return v + other.v; }
  inline number &operator+=(const number &other) { v += other.v; return *this; }
  inline number operator-() const { return -v; }
  inline number operator-(const number &other) const { return v - other.v; }
  inline number &operator-=(const number &other) { v -= other.v; return *this; }
  inline number operator*(const number &other) const { return v * other.v; }
  inline number &operator*=(const number &other) { v *= other.v; return *this; }
  inline number operator/(const number &other) const { return v / other.v; }
  inline number &operator/=(const number &other) { v /= other.v; return *this; }
  inline number operator%(const number &other) const { return v % other.v; }
  inline number &operator%=(const number &other) { v %= other.v; return *this; }
  inline bool operator==(const number &other) const { return v == other.v; }
  inline bool operator!=(const number &other) const { return v != other.v; }
  inline bool operator<(const number &other) const { return v < other.v; }
  inline bool operator>(const number &other) const { return v > other.v; }
  */

  number abs() const;
  number floor() const;
  number fract() const;
  number distance(const number &other) const;
  number recip() const;
  number sqrt() const;
  number rsqrt() const;
  number sin() const;
  number cos() const;
  number tan() const;
};

} // namespace math
} // namespace kapusha