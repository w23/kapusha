#import "AppDelegate.h"
#import "../../core/Log.h"
#import "../../sys/osx/KPView.h"

namespace fsquad {
  kapusha::IViewport *makeViewport();
}
namespace flyby {
  kapusha::IViewport *makeViewport();
}

class CocoaLog : public kapusha::Log::ISystemLog
{
public:
  virtual void write(const char* msg)
  {
    NSLog(@"%s", msg);
  }
};

@interface AppDelegate ()
@property (assign) IBOutlet KPView *viewport;
@end

@implementation AppDelegate

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  // \hack force likning KPView class from libkapusha
  [KPView class];
  
  KP_LOG_OPEN("Kapusha.log", new CocoaLog);
  [self.window setAcceptsMouseMovedEvents:YES];
  [self.viewport setViewport:flyby::makeViewport()];
}

- (BOOL)windowShouldClose:(id)sender
{
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}

@end