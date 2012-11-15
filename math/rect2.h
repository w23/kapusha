// Kapusha/math
// 2012 (c) Ivan 'w23' Avdeev, me@w23.ru

#pragma once

#include "vec2.h"

namespace math {
  template <typename T>
  struct rect2 {
    vec2<T> bl, tr;
    
    rect2() {}
    
    rect2(T v)
    : bl(v,v), tr(v,v) {}
    
    rect2(T left, T bottom, T right, T top)
    : bl(left, bottom), tr(right, top) {}
    
    rect2(vec2<T> bl_, vec2<T> tr_)
    : bl(bl_), tr(tr_) {}
    
    rect2(vec2<T> sz_)
    : bl(0), tr(sz_) {}
    
    void clear()
    {
      bl = tr = vec2<T>(0);
    }
    
    T left() const { return bl.x; }
    T bottom() const { return bl.y; }
    T right() const { return tr.x; }
    T top() const { return tr.y; }
    
    T width() const { return right() - left(); }
    T height() const { return top() - bottom(); }
    vec2<T> size() const { return vec2<T>(width(), height()); }
    T area() const { return width() * height(); }
    
    //! \fixme this should be specialization or something
    T widthi() const { return right() - left() + 1; }
    T heighti() const { return top() - bottom() + 1; }
    vec2<T> sizei() const { return vec2<T>(widthi(), heighti()); }
    T areai() const { return widthi() * heighti(); }

    vec2<T> center() const { return (bl+tr)/2; }
    
    vec2<T> bottomLeft() const { return bl; }
    vec2<T> topLeft() const { return vec2<T>(bl.x, tr.y); }
    vec2<T> topRight() const { return tr; }
    vec2<T> bottomRight() const { return vec2<T>(tr.x, bl.y); }
    
    rect2<T> operator*=(T k)
    {
      bl *= k;
      tr *= k;
      return *this;
    }
    
    bool doesContain(const vec2<T> p) const
    {
      return (p.x >= bl.x) && (p.y >= bl.y) && (p.x <= tr.x) && (p.y <= tr.y);
    }
    
    bool doesContain(const rect2<T>& r) const
    {
      return doesContain(r.bl) && doesContain(r.tr);
    }
    
    void extendToContain(const vec2<T> p)
    {
      if (bl == tr && bl == vec2<T>(0))
      {
        bl = tr = p;
        return;
      }
      if (p.x < bl.x) bl.x = p.x;
      if (p.y < bl.y) bl.y = p.y;
      if (p.x > tr.x) tr.x = p.x;
      if (p.y > tr.y) tr.y = p.y;
    }
    
    bool doesIntersect(const rect2<T>& other_rect) const
    {
      return
      (bl.x < other_rect.tr.x) &&
      (bl.y < other_rect.tr.y) &&
      (tr.x > other_rect.bl.x) &&
      (tr.y > other_rect.bl.y);
    }
    
    vec2<T> relative(const vec2<T>& v) const
    {
      return vec2<T>((v.x-bl.x)/width(), (v.y-bl.y)/height());
    }
    
    // cohen-sutherland
    enum ClipCode {
      ClipInside = 0,
      ClipLeft = 1,
      ClipRight = 2,
      ClipBottom = 4,
      ClipTop = 8
    };
    int clip_flag(const vec2<T> pt) const
    {
      int ret = ClipInside;
      if (pt.x < bl.x) ret |= ClipLeft;
      else if (pt.x > tr.x) ret |= ClipRight;
      if (pt.y < bl.y) ret |= ClipBottom;
      else if (pt.y > tr.y) ret |= ClipTop;
      return ret;
    }
    //! обрезает линию по границам прямоугольника
    //! /return < 0, если линия не попала в прямоугольник вообще
    //! /return  &1, если пришлось обрезать первую точку
    //! /return  &2, если пришлось обрезать вторую точку
    int clip_line(const vec2<T> *v_in, vec2<T> *v_out) const
    {
      v_out[0] = v_in[0];
      v_out[1] = v_in[1];
      int clip0 = clip_flag(v_out[0]);
      int clip1 = clip_flag(v_out[1]);
      int ret = 0;
      for(;;)
      {
        if ((clip0|clip1) == 0) return ret; // accept
        else if ((clip0&clip1) != 0) return -1; // reject
        
        // clip
        vec2<T> v;
        vec2<T> dv(v_out[1] - v_out[0]);
        int clip = clip0 ? clip0 : clip1;
        if (clip & ClipLeft) {
          v = vec2<T>(bl.x, v_out[0].y + (bl.x-v_out[0].x) * dv.y / dv.x);
        } else if (clip & ClipRight) {
          v = vec2<T>(tr.x, v_out[0].y + (tr.x-v_out[0].x) * dv.y / dv.x);
        } else if (clip & ClipBottom) {
          v = vec2<T>(v_out[0].x + (bl.y-v_out[0].y) * dv.x / dv.y, bl.y);
        } else {
          v = vec2<T>(v_out[0].x + (tr.y-v_out[0].y) * dv.x / dv.y, tr.y);
        }
        
        if (clip0)
        {
          v_out[0] = v;
          clip0 = clip_flag(v);
          ret |= 1;
        } else {
          v_out[1] = v;
          clip1 = clip_flag(v);
          ret |= 2;
        }
      }
    }
  };
} // namespace math