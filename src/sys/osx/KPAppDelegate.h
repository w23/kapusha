#import <Cocoa/Cocoa.h>
#import <kapusha/app.h>

@interface KPAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
- (id)initWithArgc:(int)argc argv:(const char **)argv;
@end
