#import <kapusha/app.h>
#import "KPAppDelegate.h"
#import "KPOutputManager.h"

#define KP__SYS "Cocoa"

void kp__SysLogOutput(const char *output) {
  NSLog(@"%s", output);
}

@interface KPAppDelegate () {
@private
  int argc_;
  const char **argv_;
  KPOutputManager *output_manager_;
}
@end

@implementation KPAppDelegate
- (id)initWithArgc:(int)argc argv:(const char **)argv {
  self = [super init];
  KP_ASSERT(self);
  argc_ = argc;
  argv_ = argv;
  
  output_manager_ = [[KPOutputManager alloc] init];
  [output_manager_ start];
  return self;
}

- (void)dealloc {
  [output_manager_ stop];
  output_manager_ = nil;
  [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  kpuserAppCreate(argc_, argv_);
}

- (void)applicationWillTerminate:(NSNotification *)notification {
  kpuserAppDestroy();
}
@end

KPsize kpInputsSelect(KPuptr *selectors, KPinput_o *inputs, KPsize max) {
  KP_UNUSED(selectors);
  KP_UNUSED(inputs);
  KP_UNUSED(max);
  KP__L("INPUTS NOT IMPLEMENTED");
  return 0;
}

void kpAppDestroy(int exit_code) {
  [[NSApplication sharedApplication] terminate:nil];
}
