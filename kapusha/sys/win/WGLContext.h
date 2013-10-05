#pragma once
#include <kapusha/render/Context.h>

namespace kapusha {
  class WGLContext : public Context {
  public:
    WGLContext(HDC dc);
    virtual ~WGLContext();
    virtual Context *create_shared_context() const { return nullptr; }
    virtual void make_current();

  private:
    HDC dc_;
    HGLRC glrc_;
  };
} // namespace kapusha