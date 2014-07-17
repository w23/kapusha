#import "KPSimpleView.h"
#import "KPSimpleAppDelegate.h"

@interface KPSimpleAppDelegate () {
  int argc_;
  const char **argv_;
  NSWindow *window_;
}
@end

@implementation KPSimpleAppDelegate
- (id)initWithArgc:(int)argc argv:(const char **)argv {
  self = [super init];
  if (!self) return nil;
  
  argc_ = argc;
  argv_ = argv;
  
  return self;
}

- (void)dealloc {
  [window_ release];
  [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  NSRect rect = NSMakeRect(0, 0, 1280, 720);
  
  window_ = [[NSWindow alloc] initWithContentRect:rect
                                        styleMask:15
                                          backing:NSBackingStoreBuffered
                                            defer:YES];
  window_.title = @"kapusha simpleton";
  window_.delegate = self;
  
  NSRect bounds = NSMakeRect(0, 0, window_.frame.size.width, window_.frame.size.height);
  window_.contentView = [[KPSimpleView alloc] initWithFrame:bounds argc:argc_ argv:argv_];
  
  [window_ makeKeyAndOrderFront:self];
  window_.acceptsMouseMovedEvents = YES;
}

- (BOOL)windowShouldClose:(id)sender {
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}
@end
