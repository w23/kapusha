// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru
#pragma once
#include <limits>
#include "vec2.h"

namespace kapusha {
  template <typename T> struct rect2 {
    vec2<T> min, max;
    rect2() = default;
    rect2(T v) : min(v,v), max(v,v) {}
    rect2(T left, T bottom, T right, T top)
      : min(left, bottom), max(right, top) {}
    rect2(vec2<T> min_, vec2<T> max_) : min(min_), max(max_) {}
    rect2(vec2<T> sz_) : min(0), max(sz_) {}
    template <typename Q>
    rect2(const rect2<Q> &other) : min(other.min), max(other.max) {}
    rect2 &clear() {
      min.x = min.y = std::numeric_limits<T>::max();
      max.x = max.y = std::numeric_limits<T>::min();
      return *this;
    }
    bool isValid() const { return min.x <= max.x && min.y <= max.y; }
    //bool isEmpty() const { return min.x > max.x || min.y > max.y; }
    T left() const { return min.x; }
    T bottom() const { return min.y; }
    T right() const { return max.x; }
    T top() const { return max.y; }
    T width() const { return right() - left(); }
    T height() const { return top() - bottom(); }
    vec2<T> size() const { return vec2<T>(width(), height()); }
    T area() const { return width() * height(); }
    //! \fixme this should be specialization or something
    T widthi() const { return right() - left() + 1; }
    T heighti() const { return top() - bottom() + 1; }
    vec2<T> sizei() const { return vec2<T>(widthi(), heighti()); }
    T areai() const { return widthi() * heighti(); }
    vec2<T> center() const { return (min + max) / 2; }
    vec2<T> bottomLeft() const { return min; }
    vec2<T> topLeft() const { return vec2<T>(min.x, max.y); }
    vec2<T> topRight() const { return max; }
    vec2<T> bottomRight() const { return vec2<T>(max.x, min.y); }
    rect2<T> operator*=(T k) { min *= k; max *= k; return *this; }
    rect2<T> operator+(vec2<T> v) const { return rect2(min + v, max + v); }
    bool doesContain(const vec2<T> p) const {
      return (p.x >= min.x) && (p.y >= min.y)
        && (p.x <= max.x) && (p.y <= max.y);
    }
    bool doesContain(const rect2<T> &r) const {
      return doesContain(r.min) && doesContain(r.max);
    }
    rect2 &extendToContain(const vec2<T> p) {
      if (p.x < min.x) min.x = p.x;
      if (p.y < min.y) min.y = p.y;
      if (p.x > max.x) max.x = p.x;
      if (p.y > max.y) max.y = p.y;
      return *this;
    }
    rect2 &extendToContain(const rect2<T> &r) {
      extendToContain(r.min); return extendToContain(r.max);
    }
    bool doesIntersect(const rect2<T>& other_rect) const {
      return (min.x < other_rect.max.x) && (min.y < other_rect.max.y)
        && (max.x > other_rect.min.x) && (max.y > other_rect.min.y);
    }
    vec2<T> toRelative(const vec2<T>& v) const { return (v - min) / size(); }
    vec2<T> fromRelative(const vec2<T>& v) const { return min + size() * v; }
    rect2 operator*(vec2<T> v) { min *= v; max *= v; return *this; }
  };
} // namespace kapusha
