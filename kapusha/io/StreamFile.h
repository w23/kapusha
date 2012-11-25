#pragma once

#if defined(WIN32)
#include "StreamFile_win.h"
#else
#include "StreamFile_posix.h"
#endif