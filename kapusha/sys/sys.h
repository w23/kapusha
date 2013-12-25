#pragma once

namespace kapusha {
  class IViewportFactory;
  const IViewportFactory *create_factory(/*! \todo arguments list; also, already parsed preferences*/);
} // namespace kapusha