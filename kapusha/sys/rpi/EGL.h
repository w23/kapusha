#pragma once

#include <EGL/egl.h>

namespace kapusha {
  class EGL {
  public:
    EGL();
    ~EGL();
  
    bool init(EGLNativeDisplayType display,
              EGLNativeWindowType window);
    void close();

    bool swap() const;

  private:
    void about() const;
    void fail(const char* message) const;

    EGLDisplay display_;
    EGLContext context_;
    EGLSurface surface_;
    EGLint versionMinor_, versionMajor_;

    static const EGLint s_config_attrs[];
    static const EGLint s_context_attrs[];

    struct IntString {
      int n;
      const char *s;
    };
    static const IntString s_about[];
    static const IntString s_errors[];
  };
} // namespace kapusha
