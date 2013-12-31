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
///   Does not include executable file name
/// \return Viewport factory that will be read for context parameters and
///   will be used to create a viewport.
const kapusha::IViewportFactory *kapusha_main(kapusha::core::StringArray *args);
