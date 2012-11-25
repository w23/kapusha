//
//  Event.cpp
//  Kapusha
//
//  Created by Ivan Avdeev on 22.11.12.
//
//

#include "Input.h"

namespace kapusha {
  void PointerState::mouseMove(vec2f to, u32 time)
  {
    mouseUpdate(to, Pointer::Move, 0, time);
  }
  
  void PointerState::mouseClick(vec2f at, int button, u32 time)
  {
    mouseUpdate(at, button, 0, time);
  }
  
  void PointerState::mouseUnclick(vec2f at, int button, u32 time)
  {
    mouseUpdate(at, 0, button, time);
  }
  
  void PointerState::mouseUpdate(vec2f at,
                                 int flags_add, int flags_remove,
                                 u32 time)
  {
    pointers_[0].clearEventFlags();
    
    int old_flags = pointers_[0].flags;
    
    pointers_[0].update(at, flags_add, flags_remove);
    
    combined_flags_ = pointers_[0].flags;
    changed_flags_ = old_flags ^ combined_flags_;
    
    InputState::update(time);
  }

} // namespace kapusha