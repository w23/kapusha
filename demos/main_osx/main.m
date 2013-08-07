//
//  main.m
//  kapusha-samples-osx
//
//  Created by Ivan Avdeev on 21.11.12.
//
//

#import <Cocoa/Cocoa.h>
#import "AppDelegate.h"

int main(int argc, char *argv[])
{
  NSApplication *app = [NSApplication sharedApplication];
  app.delegate = [[KapushaAppDelegate alloc] init];
  [app run];
  return 0;
}
