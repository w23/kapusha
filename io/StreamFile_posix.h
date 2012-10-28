#pragma once

#include "Stream.h"

namespace kapusha {
  
  class StreamFile : public StreamMemory {
  public:
    StreamFile() : file_(-1) {}
    virtual ~StreamFile();
    
    Error open(const char* filename);
    void close();
    
  protected:
    int file_;
  };
  
} // namespace kapusha