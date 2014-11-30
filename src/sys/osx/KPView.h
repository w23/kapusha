#import <Cocoa/Cocoa.h>

@protocol KPViewDelegate <NSObject>
- (void)viewWasInitialized;
- (void)viewWasResized:(CGSize)size;
- (void)viewWillPresent:(const CVTimeStamp*)pts;
- (void)viewWillDisappear;
@end

@interface KPView : NSOpenGLView {
@private
  id<KPViewDelegate> delegate_;
  CVDisplayLinkRef display_link_;
}
- (instancetype)initWithSize:(NSSize)size delegate:(id<KPViewDelegate>)delegate;
- (void)renderStart;
- (void)renderStop;
- (void)renderRepaint;
@end