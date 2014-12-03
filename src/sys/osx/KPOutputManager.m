#import "KPOutputManager.h"


void kp__CocoaOutputDtor(void *obj) {
  KP_THIS(KP__cocoa_output_t, obj);
  this->screen = nil;
}

KPsize kpDisplaySelect(KPuptr *selectors, KPdisplay_o *outputs, KPsize max) {
  if (max > 0) {
    NSScreen *screen = [NSScreen mainScreen];
    CGDirectDisplayID display = [[[screen deviceDescription] objectForKey:@"NSScreenNumber"] unsignedIntValue];
    KP__cocoa_output_o out = KP_NEW(KP__cocoa_output_t);
    out->parent.O.dtor = kp__CocoaOutputDtor;
    out->parent.flags = KPDisplayPrimary | KPDisplayActive;
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
    outputs[0] = (KPdisplay_o)out;
  }
  return 1;
}

@implementation KPOutputManager
- (instancetype)init {
  self = [super init];
  KP_ASSERT(self);
  return self;
}

- (void) start {
}

- (void) stop {
}

@end
