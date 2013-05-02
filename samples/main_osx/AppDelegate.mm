#import "AppDelegate.h"
#import <kapusha/core.h>
#import <kapusha/sys/osx/KPView.h>

extern kapusha::IViewport *makeViewport();

@interface AppDelegate ()
@property (assign) IBOutlet KPView *viewport;
@end

@implementation AppDelegate
- (void)dealloc { [super dealloc]; }
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  // \hack force likning KPView class from libkapusha
  [KPView class];
  
  KP_LOG_OPEN("/tmp/Kapusha.log");
  [self.window setAcceptsMouseMovedEvents:YES];
  [self.viewport setViewport:makeViewport()];
}

- (BOOL)windowShouldClose:(id)sender {
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}
@end
