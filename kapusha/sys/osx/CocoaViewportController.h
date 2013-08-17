#pragma once
#include <kapusha/viewport.h>
#include "CocoaGLContext.h"
#include "CocoaKeyState.h"
#include "CocoaPointerState.h"
#import "KPView.h"

namespace kapusha {
  class CocoaViewportController : public IViewportController {
  public:
    CocoaViewportController(KPView *view, IViewport *viewport);
    ~CocoaViewportController();

  public: // IViewportController
    void quit(int code);
    void requestRedraw();
    void setTargetFramerate(int fps);
    void setRelativeOnlyPointer(bool relative_only);
    void hideCursor(bool hide);
    const PointerState& pointerState() const;
    const KeyState& keyState() const;

    void init();
    void resize(vec2i size);
    void draw();
    void mouseMoved(NSView *view, NSEvent *event);
    void mouseDown(NSView *view, NSEvent *event);
    void mouseUp(NSView *view, NSEvent *event);
    void key(NSEvent* event, u32 time);

  private:
    KPView *view_;
    IViewport *viewport_;
    CocoaGLContext *glContext_;
    CocoaPointerState pointerState_;
    CocoaKeyState keyState_;
    MachTime time_;
    u32 prevFrameTime_;
  };

} // namespace kapusha