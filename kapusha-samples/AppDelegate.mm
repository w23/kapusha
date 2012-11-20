//
//  AppDelegate.m
//  kapusha-samples
//
//  Created by Ivan Avdeev on 10.11.12.
//
//

#import "AppDelegate.h"

#import "ViewController.h"

namespace kapusha {
  class IViewport;
}

namespace flyby {
extern kapusha::IViewport *makeViewport();
}

@implementation AppDelegate

- (void)dealloc
{
  [_viewController release];
  [_window release];
  [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
  _viewController = [[ViewController alloc] initWithViewport:flyby::makeViewport()];
  self.window.rootViewController = _viewController;
  [self.window makeKeyAndVisible];
  return YES;
}

@end
