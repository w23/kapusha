#import <Cocoa/Cocoa.h>

namespace kapusha {
  class IViewportFactory;
}

@interface KPView : NSOpenGLView
- (id) initWithFrame:(NSRect)frame
     viewportFactory:(const kapusha::IViewportFactory*)viewport_factory;
@end