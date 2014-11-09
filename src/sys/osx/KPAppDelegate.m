#import <kapusha/app.h>
#import "KPAppDelegate.h"

#define KP__SYS "Cocoa"

void kp__SysLogOutput(const char *output) {
  NSLog(@"%s", output);
}

@interface KPAppDelegate () {
  int argc_;
  const char **argv_;
}
@end

@implementation KPAppDelegate
- (id)initWithArgc:(int)argc argv:(const char **)argv {
  self = [super init];
  KP_ASSERT(self);
  argc_ = argc;
  argv_ = argv;
  return self;
}

- (void)dealloc {
  [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
  kpuserAppCreate(argc_, argv_);
}

- (void)applicationWillTerminate:(NSNotification *)notification {
  kpuserAppDestroy();
}
@end

void kp__CocoaOutputDtor(void *obj) {
  KP_THIS(KP__cocoa_output_t, obj);
  this->screen = nil;
}

KPsize kpOutputsSelect(KPuptr *selectors, KPoutput_o *outputs, KPsize max) {
  if (max > 0) {
    NSScreen *screen = [NSScreen mainScreen];
    CGDirectDisplayID display = [[[screen deviceDescription] objectForKey:@"NSScreenNumber"] unsignedIntValue];
    KP__cocoa_output_o out = KP_NEW(KP__cocoa_output_t, 0);
    out->parent.header.O.dtor = kp__CocoaOutputDtor;
    out->parent.header.name = "mainScreen";
    out->parent.flags = KPVideoOutputPrimary | KPVideoOutputActive;
    out->parent.frame_delta = 1000000000ULL / 60; /* FIXME */
    out->parent.width = screen.frame.size.width;
    out->parent.height = screen.frame.size.height;
    CGSize sizemm = CGDisplayScreenSize(display);
    out->parent.width_mm = sizemm.width;
    out->parent.height_mm = sizemm.height;
    out->parent.hppmm = (KPf32)out->parent.width_mm / out->parent.width;
    out->parent.vppmm = (KPf32)out->parent.height_mm / out->parent.height;
    out->screen = screen;
    /* TODO reasonable defaults */
    kpMemset(&out->parent.colorspace, 0, sizeof(KPcolorspace_t));
    outputs[0] = (KPoutput_o)out;
  }
  return 1;
}

KPsize kpInputsSelect(KPuptr *selectors, KPinput_o *inputs, KPsize max) {
  KP_UNUSED(selectors);
  KP_UNUSED(inputs);
  KP_UNUSED(max);
  KP__L("INPUTS NOT IMPLEMENTED");
  return 0;
}
