#import <Cocoa/Cocoa.h>

@protocol KPViewDelegate <NSObject>
- (void)viewInitialized;
- (void)viewResized:(CGSize)size;
- (void)viewPaint:(const CVTimeStamp*)pts;
- (void)viewStopping;
@end

@interface KPView : NSOpenGLView {
@private
  id<KPViewDelegate> delegate_;
  CVDisplayLinkRef display_link_;
}
- (instancetype)initWithSize:(NSSize)size delegate:(id<KPViewDelegate>)delegate;
- (void)willClose;
@end