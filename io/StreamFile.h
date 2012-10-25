#pragma once

#include <Windows.h>

#include "Stream.h"

namespace kapusha {

class StreamFile :
  public StreamSeekable
{
public:
  StreamFile(void);
  virtual ~StreamFile(void);

  Error open(const char *filename);
  void close();

private:
  HANDLE file_;
  HANDLE mapping_;
  static Error streamFileSeekFunc(StreamSeekable*, int, Reference);
};

} // namespace kapusha