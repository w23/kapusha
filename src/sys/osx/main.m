#import <Cocoa/Cocoa.h>
#import "KPAppDelegate.h"

void kp__mach_init();

int main(int argc, char *argv[]) {
  kp__mach_init();
  NSApplication *app = [NSApplication sharedApplication];
  app.delegate = [[KPAppDelegate alloc] initWithArgc:argc argv:(const char **)argv];
  [app run];
  return 0;
}
