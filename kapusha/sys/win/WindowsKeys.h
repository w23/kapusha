#pragma once
#include "windows_inc.h"
#include <kapusha/viewport/Keys.h>

namespace kapusha {
class WindowsKeys : public Keys {
public:
  bool process_event(UINT msg, WPARAM wParam, LPARAM lParam, u32 now);
private:
  static const int g_winkey_to_kapusha_[255];
}; // class WindowsKeys
} // namespace kapusha
