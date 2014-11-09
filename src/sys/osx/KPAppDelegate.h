#import <Cocoa/Cocoa.h>
#import <kapusha/app.h>

typedef struct KP__cocoa_output_o { KPoutput_video_t parent;
  NSScreen *screen;
} KP__cocoa_output_t, *KP__cocoa_output_o;

@interface KPAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
- (id)initWithArgc:(int)argc argv:(const char **)argv;
@end
