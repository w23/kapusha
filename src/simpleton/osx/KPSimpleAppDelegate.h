#import <Cocoa/Cocoa.h>

@interface KPSimpleAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
- (id)initWithArgc:(int)argc argv:(const char **)argv;
@end
