#include <kapusha/core/log.h>
#include <kapusha/core/wstring.h>
#include <kapusha/sys/sys.h>
#include "WindowController.h"
#include <shellapi.h> // CommandLineToArgvW

using kapusha::core::StringArray;

/// \todo move to sys::windows
using kapusha::WindowController;

void kapusha::core::log::sys_write(const char *message) {
  OutputDebugStringA(message);
  OutputDebugStringA("\n");
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
  UNREFERENCED_PARAMETER(hPrevInst);
  UNREFERENCED_PARAMETER(cmdLine);
  UNREFERENCED_PARAMETER(cmdShow);

  StringArray::shared args(new StringArray());
  int wargc = 0;
  LPWSTR *wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);

  if (wargc > 0) {
    args->reserve(wargc);
    for (int i = 0; i < wargc; ++i)
      args->push_back(kapusha::core::windows::wstring_t::create_string_from_wchar(wargv[i]));
  }

  LocalFree(wargv);

  WindowController controller(hInst, ::kapusha_main(args));
  return controller.run();
}
