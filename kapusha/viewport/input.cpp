#include "input.h"

namespace kapusha {
  void PointerState::beginUpdate() {
    for (int i = 0; i < KAPUSHA_MAX_POINTERS_IN_EVENT; ++i) pointers_[i].prepare();
  }
  void PointerState::endUpdate(u32 time) {
    for (int i = 0; i < KAPUSHA_MAX_POINTERS_IN_EVENT; ++i) {
      flagsCombined_ |= pointers_[i].flags_;
      flagsChanged_ |= pointers_[i].flagsChanged_;
      //! \todo calculate center and area
    }
    InputState::update(time);
  }
  void PointerState::pointerMoveTo(int index, vec2f w_at,
                                   int flags_add, int flags_remove) {
    pointers_[index].updateWithPosition((w_at - shift_) * scale_, flags_add, flags_remove);
  }
  void PointerState::pointerMoveBy(int index, vec2f w_by,
                                   int flags_add, int flags_remove) {
    pointers_[index].updateWithDelta(w_by * scale_, flags_add, flags_remove);
  }
  void PointerState::mouseMoveTo(vec2f w_to, u32 time) {
    beginUpdate();
    pointerMoveTo(0, w_to, Pointer::Moved, 0);
    endUpdate(time);
  }
  void PointerState::mouseMoveBy(vec2f w_by, u32 time) {
    beginUpdate();
    pointerMoveBy(0, w_by, Pointer::Moved, 0);
    endUpdate(time);
  }
  void PointerState::mouseDown(int button, u32 time) {
    beginUpdate();
    pointers_[0].updateFlags(button, 0);
    endUpdate(time);
  }
  void PointerState::mouseUp(int button, u32 time) {
    beginUpdate();
    pointers_[0].updateFlags(0, button);
    endUpdate(time);
  }
} // namespace kapusha
