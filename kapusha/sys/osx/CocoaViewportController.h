#pragma once
#include <memory>
#include <kapusha/viewport.h>
#include "CocoaGLContext.h"
#include "CocoaKeys.h"
#include "CocoaPointers.h"
#import "KPView.h"

namespace kapusha {
  class CocoaViewportController : public IViewportController {
  public:
    CocoaViewportController(KPView *view, const IViewportFactory *factory);
    ~CocoaViewportController();

  public: // IViewportController
    void quit(int code);
    void grab_input(bool grab);
    const Pointers& pointers() const;
    const Keys& keys() const;

    void init();
    void resize(vec2i size);
    void draw();
    void mouse_moved(NSView *view, NSEvent *event);
    void mouse_down(NSView *view, NSEvent *event);
    void mouse_up(NSView *view, NSEvent *event);
    void key(NSEvent* event, u32 time);

  private:
    KPView *view_;
    const IViewportFactory *factory_;
    std::unique_ptr<IViewport> viewport_;
    std::unique_ptr<CocoaGLContext> context_;
    CocoaPointers pointers_;
    CocoaKeys keys_;
    MachTime time_;
    u32 prevFrameTime_;
  };

} // namespace kapusha