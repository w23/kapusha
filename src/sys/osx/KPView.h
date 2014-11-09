#import <Cocoa/Cocoa.h>
#import "KPAppDelegate.h"
#import "KPWindow.h"
#import <kapusha/window.h>

@class KPView;

typedef struct KP__cocoa_window_t { KP_O;
  void *user_data;
  KPwindow_painter_create_f painter_create_func;
  KPwindow_painter_configure_f painter_configure_func;
  KPwindow_painter_f painter_func;
  KPwindow_painter_destroy_f painter_destroy_func;
  
  KP__cocoa_output_o output;

  KPu32 width, height;
  KPView *view;
} KP__cocoa_window_t, *KP__cocoa_window_o;



@interface KPView : NSOpenGLView
- (id) initWithWindow:(KP__cocoa_window_o)window;
@end
