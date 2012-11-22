#include "Log.h"
#include "IViewport.h"

namespace kapusha {
  void IViewport::userEvent(const Event &event)
  {
    switch (event.type())
    {
      case Event::Pointer:
        pointerEvent(static_cast<const PointerEvent&>(event));
        break;
      case Event::Key:
        keyEvent(static_cast<const KeyEvent&>(event));
        break;
      case Event::TextInput:
        textInputEvent(static_cast<const TextInputEvent&>(event));
        break;
      default:
        L("Unknown user event type %d", event.type());
    }
  }
}