#include <windows.h>
#include <kapusha/core.h>
#include <kapusha/sys/win/Window.h>

using namespace kapusha;

extern IViewport *makeViewport();

void log::sys_write(const char *message) {
  OutputDebugStringA(message);
  OutputDebugStringA("\n");
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInst);
  UNREFERENCED_PARAMETER(cmdLine);
  UNREFERENCED_PARAMETER(cmdShow);

  return RunWindow(hInst, makeViewport(), 800, 600, false);
}