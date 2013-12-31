#import "KPAppDelegate.h"
#import <kapusha/core.h>
#import <kapusha/viewport.h>
#import <kapusha/sys/sys.h>
#import <kapusha/sys/osx/KPView.h>

@interface KPAppDelegate () {
  NSWindow *window_;
}
@end

@implementation KPAppDelegate
- (void)dealloc { [super dealloc]; }

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  NSArray *ns_args = [[NSProcessInfo processInfo] arguments];
  kapusha::core::StringArray::shared args =
    new kapusha::core::StringArray([ns_args count]);
  for (NSString *arg in ns_args)
    args->push_back(new kapusha::core::String([arg UTF8String]));
  
  const kapusha::IViewportFactory *viewport_factory = kapusha_main(args.get());
  
  const kapusha::IViewportFactory::Preferences &prefs
    = viewport_factory->preferences();
  
  kapusha::vec2i size = prefs.prefer_resolution;
  if (size.x <= 0 || size.y <= 0) size = kapusha::vec2i(640, 480);
  
  NSRect rect = NSMakeRect(0, 0, size.x, size.y);
  
  window_ = [[NSWindow alloc] initWithContentRect:rect
                                        styleMask:15
                                          backing:NSBackingStoreBuffered
                                            defer:YES];
  window_.title = [NSString stringWithCString:prefs.window_title
                                     encoding:NSUTF8StringEncoding];
  window_.delegate = self;
  
  NSRect bounds = NSMakeRect(0, 0, window_.frame.size.width, window_.frame.size.height);
  window_.contentView = [[KPView alloc] initWithFrame:bounds
                                         viewportFactory:viewport_factory];
  
  [window_ makeKeyAndOrderFront:self];
  window_.acceptsMouseMovedEvents = YES;
}

- (BOOL)windowShouldClose:(id)sender {
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}
@end
