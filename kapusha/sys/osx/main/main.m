#import <Cocoa/Cocoa.h>
#import "KPAppDelegate.h"

int main(int argc, char *argv[])
{
  NSApplication *app = [NSApplication sharedApplication];
  app.delegate = [[KPAppDelegate alloc] init];
  [app run];
  return 0;
}
