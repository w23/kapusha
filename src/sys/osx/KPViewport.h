//
//  KPViewport.h
//  Kapusha
//
//  Created by Ivan Avdeev on 22.11.12.
//
//

#import <Cocoa/Cocoa.h>

namespace kapusha {
  class IViewport;
}

@interface KPViewport : NSOpenGLView
- (void) setViewport:(kapusha::IViewport*)viewport;
@end