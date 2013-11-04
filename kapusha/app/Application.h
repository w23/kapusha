#pragma once

namespace kapusha {
class IViewport;
class IViewportFactory;

//! Kapusha-powered application startup settings
struct Application {
  const IViewportFactory *viewport_factory;
};

extern Application the_application;

} // namespace kapusha
