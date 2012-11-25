//
//  main.m
//  kapusha-samples
//
//  Created by Ivan Avdeev on 10.11.12.
//
//

#include "../sys/Log.h"
#import <UIKit/UIKit.h>

#import "AppDelegate.h"

class iOSLog : public kapusha::Log::ISystemLog
{
  virtual void write(const char* string)
  {
    NSLog(@"%s", string);
  }
};

int main(int argc, char *argv[])
{
  KP_LOG_OPEN(0, new iOSLog);
  @autoreleasepool {
      return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
  }
}
