#import "AppDelegate.h"
#import <kapusha/core.h>
#import <kapusha/sys/osx/KPView.h>

extern kapusha::IViewport *makeViewport();

@interface KapushaAppDelegate ()
{
  NSWindow *window_;
}
@end

@implementation KapushaAppDelegate
- (void)dealloc { [super dealloc]; }

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  KP_LOG_OPEN("/tmp/Kapusha.log");
  
  window_ = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 800, 600)
                                        styleMask:15
                                          backing:NSBackingStoreBuffered
                                            defer:YES];
  window_.delegate = self;
  
  NSRect bounds = NSMakeRect(0, 0, window_.frame.size.width, window_.frame.size.height);
  window_.contentView = [[KPView alloc] initWithFrame:bounds
                                         withViewport:makeViewport()];
  
  [window_ makeKeyAndOrderFront:self];
  window_.acceptsMouseMovedEvents = YES;
}

- (BOOL)windowShouldClose:(id)sender {
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}
@end
