#import <Cocoa/Cocoa.h>

namespace kapusha {
  class IViewport;
}

@interface KPView : NSOpenGLView
- (id) initWithFrame:(NSRect)frame WithViewport:(kapusha::IViewport*)viewport;
- (void) setViewport:(kapusha::IViewport*)viewport;
@end