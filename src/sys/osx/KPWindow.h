#import <Cocoa/Cocoa.h>
#import "kapusha/window.h"
#import "KPView.h"
#import "KPOutputManager.h"

@class KPWindow;

typedef struct KP__cocoa_window_t { KP_O;
  __unsafe_unretained KPWindow *window;
} KP__cocoa_window_t, *KP__cocoa_window_o;

@interface KPWindow : NSWindow <KPViewDelegate,NSWindowDelegate> {
@private
  KP__cocoa_window_t *window_;
  KP__cocoa_output_o output_;
  KPuptr queue_seq_;
  KPmessage_queue_t queue_;
  KPmessage_user_t queue_userdata_;
  KPwindow_painter_f painter_;
  KPwindow_painter_event_t pevent_;
}
- (instancetype)
  initWithParams:(const KPwindow_create_params_t*)params
  window:(KP__cocoa_window_t*)window;
@end
