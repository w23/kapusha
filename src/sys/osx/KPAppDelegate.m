#import <kapusha/app.h>
#import "KPAppDelegate.h"
#import "KPOutputManager.h"

#define KP__SYS "CocoaApp"

void kp__SysLogOutput(const char *output) {
  NSLog(@"%s", output);
}

@interface KPAppDelegate () {
@private
  int argc_;
  const char **argv_;
  KPOutputManager *output_manager_;
  KPthread_t app_thread_;
  enum {
    KPAppIdle,
    KPAppRunning,
    KPAppTerminated
  } state;
}
- (void) kapushaAppTerminatedWithCode:(int)code;
- (int) argc;
- (const char**) argv;
@end


static void *app_thread(void* arg) {
  KPAppDelegate *delegate = (KPAppDelegate*)arg;
  int retcode = kpuserAppRun([delegate argc], [delegate argv]);
  [delegate kapushaAppTerminatedWithCode:retcode];
  return 0;
}

@implementation KPAppDelegate
- (int) argc {
  return argc_;
}

- (const char**) argv {
  return argv_;
}

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
  KPthread_params_t params;
  params.name = "app";
  params.thread_func = app_thread;
  params.user_data = self;
  kpThreadSpawn(&params);
}

- (void)applicationWillTerminate:(NSNotification *)notification {
  KP__L("%s", __FUNCTION__);
}

- (void) kapushaAppTerminatedWithCode:(int)code {
  KP__L("App terminated with code: %d", code);
  [[NSApplication sharedApplication] terminate:self];
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
