//
//  AppDelegate.m
//  kapusha-samples-osx
//
//  Created by Ivan Avdeev on 21.11.12.
//
//

#import "AppDelegate.h"
#import "../../sys/osx/KPView.h"

namespace fsquad {
  kapusha::IViewport *makeViewport();
}
namespace flyby {
  kapusha::IViewport *makeViewport();
}


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
  [self.window setAcceptsMouseMovedEvents:YES];
  [self.viewport setViewport:fsquad::makeViewport()];
}

- (BOOL)windowShouldClose:(id)sender
{
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}

@end
