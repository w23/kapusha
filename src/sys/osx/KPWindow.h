#import <Cocoa/Cocoa.h>
#import "kapusha/window.h"
#import "KPView.h"
#import "KPOutputManager.h"

@class KPWindow;

typedef struct KP__cocoa_window_t { KP_O;
  enum {
    KPCocoaWindowIdle,
    KPCocoaWindowCreated,
    KPCocoaWindowRunning
  } state;
  void *user_data;
  KPwindow_painter_f painter_func;
  KPstring_o title;
  KPmessage_queue_t queue;
  KPu32 queue_tag;
  KP__cocoa_output_o output;
  __unsafe_unretained KPWindow *window;
} KP__cocoa_window_t, *KP__cocoa_window_o;

@interface KPWindow : NSWindow <KPViewDelegate,NSWindowDelegate> {
@private
  KP__cocoa_window_t *window_;
  KPwindow_painter_event_t pevent_;
}
- (instancetype)initWithWindow:(KP__cocoa_window_t*)window
  floatingParams:(const KPwindow_floating_params_t*)params;
- (instancetype)initWithWindow:(KP__cocoa_window_t*)window
  output:(KP__cocoa_output_o)output;
@end
