#include "X11Pointers.h"
#include "X11Keys.h"

namespace kapusha {
namespace sys {

class GLXViewportController : public IViewportController {
public:
  inline GLXViewportController() : display_(0), viewport_(nullptr), size_(0) {}
  ~GLXViewportController();
  int run(const IViewportFactory *factory);

public: // IViewportController
  void quit(int code) {}
  void requestRedraw() {}
  void setRelativeOnlyPointer(bool relative_only) {}
  void hideCursor(bool hide) {}
  const PointerState& pointerState() const { return pointers_; }
  const KeyState& keyState() const { return keys_; }

private:
  void event_loop();

  ::Display *display_;
  ::Window window_;
  IViewport *viewport_;
  vec2i size_;
  X11Pointers pointers_;
  X11Keys keys_;
};

} // namespace sys
} // namespace kapusha
