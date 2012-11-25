//
//  ViewController.m
//  kapusha-samples
//
//  Created by Ivan Avdeev on 10.11.12.
//
//

#import "ViewController.h"

class iOSSystem : public kapusha::ISystem
{
public: // ISystem
  virtual void quit(int code);
  virtual void redraw();
  virtual void pointerReset() {}
  
  void setController(ViewController* controller)
  {
    controller_ = controller;
  }
  
private:
  ViewController* controller_;
};

////////////////////////////////////////////////////////////////////////////////

@interface ViewController () {
  kapusha::IViewport *viewport_;
  iOSSystem sys_;
  BOOL draw_;
}
@property (strong, nonatomic) EAGLContext *context;

- (void) setupGL;
- (void) tearDownGL;

- (void) redraw;
- (void) quit;
@end

void iOSSystem::quit(int code)
{
  [controller_ quit];
}

void iOSSystem::redraw()
{
  [controller_ redraw];
}

////////////////////////////////////////////////////////////////////////////////

@implementation ViewController

- (id)initWithViewport:(kapusha::IViewport *)viewport
{
  self = [super init];
  if (!self) return nil;
  
  viewport_ = viewport;
  sys_.setController(self);
  
  return self;
}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
    [_context release];
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
      
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
  [EAGLContext setCurrentContext:self.context];
    
  viewport_->init(&sys_);
}

- (void)tearDownGL
{
  [EAGLContext setCurrentContext:self.context];
  viewport_->close();
}

- (void) redraw
{
  draw_ = YES;
  if (self.paused)
    self.paused = NO;
}

- (void) quit
{
  if (self.navigationController)
    [self.navigationController popViewControllerAnimated:YES];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
  viewport_->resize(self.view.bounds.size.width, self.view.bounds.size.height);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
  draw_ = NO;
  viewport_->draw(self.timeSinceFirstResume, self.timeSinceLastDraw);
  if (!draw_)
    self.paused = YES;
}

@end
