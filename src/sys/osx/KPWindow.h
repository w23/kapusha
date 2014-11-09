#import <Cocoa/Cocoa.h>

@interface KPWindow : NSWindow
- (instancetype) initWithRect:(NSRect)rect
                     onScreen:(NSScreen*)screen
                     delegate:(id<NSWindowDelegate>)delegate;
@end
