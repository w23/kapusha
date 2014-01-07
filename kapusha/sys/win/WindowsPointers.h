#include <kapusha/viewport/Pointers.h>
#include "windows_inc.h"

namespace kapusha {
class WindowsPointers : public Pointers {
public:
  WindowsPointers() : grabbed_(false), ignore_(false) {}
  bool process_event(UINT msg, WPARAM wParam, LPARAM lParam, u32 now);
  void resize(math::vec2i size);
  void grab(bool grab_in, HWND window);
    
private:
  void grab();
  inline static vec2f lparam_to_vec2f(LPARAM lParam) {
    return vec2f(static_cast<float>(GET_X_LPARAM(lParam)),
                 static_cast<float>(GET_Y_LPARAM(lParam)));
  }

  HWND window_;
  math::vec2i size_;
  math::vec2f center_;
  bool grabbed_, ignore_;
};
} // namespace kapusha