#include "extern.h"

namespace kapusha {
namespace sys {

  class X11Keys : public Keys {
  public:
	X11Keys() {}
    bool process(const XEvent &event);
  };

} // namespace sys
} // namespace kapusha
