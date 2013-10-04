#include <kapusha/sys/win/Window.h>
#include <kapusha/core/Log.h>
#include <kapusha/app.h>

using namespace kapusha;

void log::sys_write(const char *message) {
  OutputDebugStringA(message);
  OutputDebugStringA("\n");
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
  UNREFERENCED_PARAMETER(hPrevInst);
  UNREFERENCED_PARAMETER(cmdLine);
  UNREFERENCED_PARAMETER(cmdShow);

  return RunWindow(hInst, the_application.viewport_factory);
}