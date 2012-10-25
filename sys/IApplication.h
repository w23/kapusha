#pragma once

namespace kapusha {

  class ISystem {
  public:
    virtual void quit(int code = 0) = 0;
    virtual void redraw() = 0;
  };

  class IApplication {
  public:
    virtual ~IApplication() {}
    virtual void init(ISystem* system) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void draw(int ms) = 0;
  };

} // namespace kapusha