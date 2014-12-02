#import <Cocoa/Cocoa.h>
#import "kapusha/app.h"

typedef struct KP__cocoa_output_o { KPdisplay_t parent;
  NSScreen *screen;
} KP__cocoa_output_t, *KP__cocoa_output_o;

@interface KPOutputManager : NSObject
- (instancetype)init;
- (void) start;
- (void) stop;
@end
