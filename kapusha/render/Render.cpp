#include "../core/Core.h"
#include "Render.h"

namespace kapusha {

  Render *Render::current_render_ = 0;

  Render *Render::currentRender() {
    return current_render_;
  }

  Render::Render()
    : cullFace_(GL_CULL_FACE)
    , depthTest_(GL_DEPTH_TEST)
    , blend_(GL_BLEND)
    , bufferArray_(GL_ARRAY_BUFFER)
    , bufferIndex_(GL_ELEMENT_ARRAY_BUFFER)
    , materialCurrent_(0)
    , materialNew_(0)
  {
    KP_ASSERT(!current_render_);
    current_render_ = this;
  }

  Render::~Render()
  {
    current_render_ = 0;
  }

  void Render::commit()
  {
    // material is the first one as it can affect other states
    if (materialCurrent_ != materialNew_)
    {
      materialCurrent_ = materialNew_;
    }
    if (materialCurrent_)
      materialCurrent_->use(this);

    //! \fixme array!
    cullFace_.commit();
    depthTest_.commit();
    blend_.commit();

    bufferArray_.commit();
    bufferIndex_.commit();

    texture_.commit();
  }

} // namespace kapusha
