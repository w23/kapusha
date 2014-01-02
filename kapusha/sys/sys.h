#pragma once
#include <kapusha/core/String.h>

namespace kapusha {
class IViewportFactory;
} // namespace kapusha

/// \brief main() function for kapusha/sys-based apps
///
/// This is the main entry point for application that use kapusha/sys to
/// initialize OpenGL and user input on different platforms.
/// \param args List of provided command line arguments
/// \return Viewport factory that will be used for creating app viewport
const kapusha::IViewportFactory *kapusha_main(kapusha::core::StringArray *args);
