#import <Cocoa/Cocoa.h>
#import "KPSimpleAppDelegate.h"

void kp__mach_init();

int main(int argc, char *argv[]) {
  kp__mach_init();
  NSApplication *app = [NSApplication sharedApplication];
  app.delegate = [[KPSimpleAppDelegate alloc] init];
  [app run];
  return 0;
}
