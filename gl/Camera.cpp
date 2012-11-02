#include "Camera.h"

namespace kapusha {
  Camera::Camera()
    : position_(0.f)
    , forward_(0.f, 0.f, -1.f)
    , right_(1.f, 0.f, 0.f)
  {
  }

  Camera::~Camera()
  {
  }
} // namespace kapusha