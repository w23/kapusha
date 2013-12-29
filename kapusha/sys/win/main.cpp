#include <kapusha/core/log.h>
#include <kapusha/sys/sys.h>
#include "WindowController.h"
#include "WString.h"
#include <shellapi.h> // CommandLineToArgvW

using namespace kapusha;

void log::sys_write(const char *message) {
  OutputDebugStringA(message);
  OutputDebugStringA("\n");
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
  UNREFERENCED_PARAMETER(hPrevInst);
  UNREFERENCED_PARAMETER(cmdLine);
  UNREFERENCED_PARAMETER(cmdShow);

  shared<ObjectArrayOf<String> > args(new ObjectArrayOf<String>());
  int wargc = 0;
  LPWSTR *wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);

  if (wargc > 0) {
    args->reserve(wargc);
    for (int i = 0; i < wargc; ++i)
      args->push_back(WString::toString(wargv[i]));
  }

  LocalFree(wargv);

  WindowController controller(hInst, ::kapusha_main(args.get()));
  return controller.run();
}
