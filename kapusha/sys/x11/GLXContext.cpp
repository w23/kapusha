#include "GLXContext.h"

namespace kapusha {
namespace sys {

GLXContext::GLXContext(::Display *display,
                       ::GLXFBConfig fbconfig, ::GLXDrawable drawable)
  : display_(display), fbconfig_(fbconfig), drawable_(drawable) {
  init(nullptr);
}

GLXContext::GLXContext(const GLXContext *share)
  : display_(share->display_), fbconfig_(share->fbconfig_)
  , drawable_(0) {
  init(share);
}

void GLXContext::init(const GLXContext *share) {
  //! \todo glXCreateContextAttribsARB
  context_ = glXCreateNewContext(display_, fbconfig_, GLX_RGBA_TYPE,
                                 share ? share->context_ : NULL, True);
  KP_ASSERT(context_ != NULL);
}

GLXContext::~GLXContext() {
  //! \todo unmake current
  glXDestroyContext(display_, context_);
}

Context *GLXContext::create_shared() const {
  return new GLXContext(this);
}

void GLXContext::make_current() {
  //! \todo glXMakeContextCurrent
  KP_ENSURE(True == glXMakeCurrent(display_, drawable_, context_));
  Context::make_current();
}

} // namespace sys
} // namespace kapusha
