//
//  KPViewport.m
//  Kapusha
//
//  Created by Ivan Avdeev on 22.11.12.
//
//

#import "KPViewport.h"
#import "../../core/IViewport.h"

class CocoaSystem;

@interface KPViewport ()
{
  CocoaSystem *system_;
  kapusha::IViewport *viewport_;
  BOOL redraw_;
}
//- (void) redraw;
@end

class CocoaSystem : public kapusha::ISystem
{
public:
  CocoaSystem() {}
  
  virtual void quit(int code) {}
  virtual void redraw() {
//    viewport_.redraw_;
  }
  virtual void pointerReset() {}
  
private:
  KPViewport *viewport_;
};


@implementation KPViewport

- (void) dealloc
{
  delete system_;
  [super dealloc];
}

- (void) setViewport:(kapusha::IViewport*)viewport
{
  if (viewport_)
  {
    viewport_->close();
  }
  viewport_ = viewport;

  if ([self openGLContext])
  {
    [self prepareOpenGL];
    [self reshape];
    [self drawRect:[self bounds]];
  }
}

- (void) prepareOpenGL
{
  if (!system_)
    system_ = new CocoaSystem;
  
  if (viewport_)
    viewport_->init(system_);
}

- (void) reshape
{
  if (viewport_)
    viewport_->resize(kapusha::vec2i(self.bounds.size.width,
                                     self.bounds.size.height));
}

- (void)drawRect:(NSRect)dirtyRect
{
  if (viewport_)
  {
    viewport_->draw(1.f, .1f);
    [[self openGLContext] flushBuffer];
  }
}

@end
