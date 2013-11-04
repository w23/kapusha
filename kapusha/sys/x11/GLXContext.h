#pragma once
#include "extern.h"

namespace kapusha {
namespace sys {

class GLXContext : public Context {
public:
  //! Create a new context that renders to drawable
  GLXContext(::Display *display, ::GLXFBConfig fbconfig, ::GLXDrawable drawable);

  //! Create a context that renders to nowhere, but shares data with another
  //! Will use teh same display and fbconfig. Will set drawable to none
  //! \warning Contexts that have no default framebuffer are >3.0 feature
  //!          ...and I just expect it to magically work on desktop lols.
  //! \param share A previously created context to share resources with
  GLXContext(const GLXContext *share);
  virtual ~GLXContext();
  virtual Context *create_shared() const;
  virtual void make_current();
private:
  void init(const GLXContext *share);
  //! \warning all these are managed outside
  //!          shared context gets same values (and the same objects)
  //!          so one should be careful and release all shared contexts before
  //!          the main one
  ::Display *display_;
  ::GLXFBConfig fbconfig_;
  ::GLXDrawable drawable_;
  ::GLXContext context_;
}; // class GLXContext

} // namespace sys
} // namespace kapusha
