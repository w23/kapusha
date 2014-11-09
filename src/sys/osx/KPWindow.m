#import "KPWindow.h"
#import "KPView.h"

@interface KPWindow ()
@end

@implementation KPWindow
- (instancetype) initWithRect:(NSRect)rect
                     onScreen:(NSScreen*)screen
                     delegate:(id<NSWindowDelegate>)delegate;
{
  self = [super initWithContentRect:rect
                          styleMask:15
                            backing:NSBackingStoreBuffered
                              defer:YES
                             screen:screen];
  KP_ASSERT(self);
  return self;
}
@end
