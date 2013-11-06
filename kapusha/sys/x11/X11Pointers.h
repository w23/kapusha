#include "extern.h"

namespace kapusha {
namespace sys {

  class X11Pointers : public Pointers {
  public:
	X11Pointers() {}
    void resize(vec2i size) {
      evt_resize_viewport(
        vec2f(0.f, static_cast<float>(size.y)),
        vec2f(static_cast<float>(size.x), 0.f));
    }
    bool process(const XEvent &event);
  };

} // namespace sys
} // namespace kapusha
