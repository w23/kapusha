#pragma once

#include <Windows.h>

#include "Stream.h"

namespace kapusha {

class StreamFile : public StreamMemory
{
public:
  StreamFile(void);
  virtual ~StreamFile(void);

  Error open(const char *filename);
  void close();

private:
  HANDLE file_;
  HANDLE mapping_;
  static Error streamFileSeekFunc(Stream*, int, Reference);
};

} // namespace kapusha