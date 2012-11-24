#import <Cocoa/Cocoa.h>

namespace kapusha {
  class IViewport;
}

@interface KPView : NSOpenGLView
- (void) setViewport:(kapusha::IViewport*)viewport;
@end