#import "KPAppDelegate.h"
#import <kapusha/core.h>
#import <kapusha/app.h>
#import <kapusha/sys/osx/KPView.h>

@interface KPAppDelegate ()
{
  NSWindow *window_;
}
@end

@implementation KPAppDelegate
- (void)dealloc { [super dealloc]; }

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  
  NSRect rect = NSMakeRect(0, 0,
                           kapusha::the_application.prefer_resolution.x,
                           kapusha::the_application.prefer_resolution.y);
  
  window_ = [[NSWindow alloc] initWithContentRect:rect
                                        styleMask:15
                                          backing:NSBackingStoreBuffered
                                            defer:YES];
  window_.delegate = self;
  
  NSRect bounds = NSMakeRect(0, 0, window_.frame.size.width, window_.frame.size.height);
  window_.contentView = [[KPView alloc] initWithFrame:bounds
                                         withViewport:kapusha::the_application.createViewport()];
  
  [window_ makeKeyAndOrderFront:self];
  window_.acceptsMouseMovedEvents = YES;
}

- (BOOL)windowShouldClose:(id)sender {
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}
@end
