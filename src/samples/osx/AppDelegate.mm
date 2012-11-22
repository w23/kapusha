//
//  AppDelegate.m
//  kapusha-samples-osx
//
//  Created by Ivan Avdeev on 21.11.12.
//
//

#import "AppDelegate.h"
#import "../../sys/osx/KPViewport.h"

namespace fsquad {
  kapusha::IViewport *makeViewport();
}
namespace flyby {
  kapusha::IViewport *makeViewport();
}


@interface AppDelegate ()
@property (assign) IBOutlet KPViewport *viewport;
@end

@implementation AppDelegate

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
  [self.viewport setViewport:flyby::makeViewport()];
}

- (BOOL)windowShouldClose:(id)sender
{
  [[NSApplication sharedApplication] terminate:self];
  return YES;
}

@end
