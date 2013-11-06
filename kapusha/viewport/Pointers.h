#pragma once
#include "Input.h"
#include "../core/limits.h"
#include "../math.h"

namespace kapusha {
  //! State of pointer-type input
  class Pointers : public Input {
  public:
    //! Single pointer
    struct Pointer {
      enum Flags {
        None = 0, //! pointer is inactive
        Moved = 1, //! pointer has moved (event flag)
        Pressed = 2, //! touch down or 
        LeftButton = Pressed, //! left mouse button
        RightButton = 4,
        MiddleButton = 8,
        Cancelled = 16, //! pointer was cancelled (event flag)
        AnyButton = LeftButton | RightButton | MiddleButton,
        _Event = Moved | Cancelled,
      };

      inline bool is_pressed() const { return (flags_ & AnyButton) != 0; }
      inline bool is_left_pressed() const { return (flags_ & LeftButton) != 0; }
      inline bool is_right_pressed() const { return (flags_ & RightButton) != 0; }
      inline bool is_middle_pressed() const { return (flags_ & MiddleButton) != 0;}
      
      inline bool was_pressed(Flags button = AnyButton) const {
        return is_pressed() && (flags_change_ & button);
      }
      inline bool was_released(Flags button = AnyButton) const {
        return !is_pressed() && (flags_change_ & button);
      }
      inline bool was_moved() const { return (flags_ & Moved) != 0; }
      inline bool was_cancelled() const { return (flags_ & Cancelled) != 0; }

      inline vec2f position() const { return point_; }
      inline vec2f delta() const { return point_delta_; }

    //protected: friend class PointerState;
      int flags_, flags_change_;
      vec2f point_, point_delta_; //! in normalized viewport coordinates

      Pointer() : flags_(0), flags_change_(0), point_(0), point_delta_(0) {}
      inline void evt_prepare() { flags_ ^= _Event; }
      inline void evt_clear() { flags_ = 0; }
      inline void evt_update_flags(int flags_add, int flags_remove = 0) {
        int newflags = (flags_ | flags_add) ^ flags_remove;
        flags_change_ = flags_ ^ newflags, flags_ = newflags;
      }
      void evt_update_with_delta(vec2f delta, int flags_add, int flags_remove = 0) {
        point_delta_ = delta, point_ += delta;
        evt_update_flags(flags_add, flags_remove);
      }
      void evt_update_with_position(vec2f position, int flags_add, int flags_remove = 0) {
        point_delta_ = position - point_, point_ = position;
        evt_update_flags(flags_add, flags_remove);
      }
    }; // struct Pointer

  public: // IViewport user interface
    inline const Pointer& main() const { return pointers_[0]; }
    inline bool is_pressed() const { return main().is_pressed(); }
    inline bool is_left_pressed() const { return main().is_left_pressed(); }
    inline bool is_right_pressed() const { return main().is_right_pressed(); }
    inline bool is_middle_pressed() const { return main().is_middle_pressed(); }
    inline bool was_pressed() const { return main().was_pressed(); }
    inline bool was_released() const { return main().was_released(); }
    inline bool was_cancelled() const { return main().was_cancelled(); }

  protected: // implementers
    Pointers()
      : flags_combined_(0), flags_change_(0), scale_(1.f), shift_(0.f) {}

    inline void evt_resize_viewport(vec2f bottom_left, vec2f top_right) {
      scale_ = vec2f(2.f) / (top_right - bottom_left);
      shift_ = vec2f(1.f) - top_right * scale_;
    }

    inline void evt_begin_update() {
      for (int i = 0; i < KAPUSHA_MAX_POINTERS_IN_EVENT; ++i) pointers_[i].evt_prepare();
    }

    inline void evt_end_update(u32 time) {
      for (int i = 0; i < KAPUSHA_MAX_POINTERS_IN_EVENT; ++i) {
        flags_combined_ |= pointers_[i].flags_;
        flags_change_ |= pointers_[i].flags_change_;
        //! \todo calculate center and area
      }
      Input::evt_update(time);
    }

    inline void evt_pointer_move_to(int index, vec2f w_at,
                                    int flags_add, int flags_remove) {
      pointers_[index].evt_update_with_position(w_at * scale_ + shift_,
                                                flags_add, flags_remove);
    }

    inline void evt_pointer_move_by(int index, vec2f w_by,
                                    int flags_add, int flags_remove) {
      pointers_[index].evt_update_with_delta(w_by * scale_, flags_add, flags_remove);
    }

    inline void evt_mouse_move_to(vec2f w_to, u32 time) {
      evt_begin_update();
      evt_pointer_move_to(0, w_to, Pointer::Moved, 0);
      evt_end_update(time);
    }
    inline void evt_mouse_move_by(vec2f w_by, u32 time) {
      evt_begin_update();
      evt_pointer_move_by(0, w_by, Pointer::Moved, 0);
      evt_end_update(time);
    }
    inline void evt_mouse_down(int button, u32 time) {
      evt_begin_update();
      pointers_[0].evt_update_flags(button, 0);
      evt_end_update(time);
    }
    inline void evt_mouse_up(int button, u32 time) {
      evt_begin_update();
      pointers_[0].evt_update_flags(0, button);
      evt_end_update(time);
    }

    int flags_combined_;
    int flags_change_;

    vec2f scale_, shift_;
    Pointer pointers_[KAPUSHA_MAX_POINTERS_IN_EVENT];
  }; // class Pointers
} // namespace kapusha
