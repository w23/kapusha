#import <Cocoa/Cocoa.h>

namespace kapusha {
  class IViewport;
}

@interface KPView : NSOpenGLView
- (id) initWithFrame:(NSRect)frame withViewport:(kapusha::IViewport*)viewport;
@end