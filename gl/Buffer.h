#pragma once
#include "../sys/IShared.h"

namespace kapusha {

  class Stream;
  
  class Buffer : public IShared {
  public:
    enum Binding {
      BindingArray,
      BindingIndex
    };
    
  public:
    Buffer();
    ~Buffer();

    void load(Stream* data, unsigned size);
    void load(void* data, unsigned size);
    
    void bind(Binding binding = BindingArray) const;

  private: // noncopyable
    Buffer& operator=(const Buffer& right) { return *this; }
    Buffer(const Buffer& right) {}

  private:
    unsigned name_;
  };
  
  typedef shared<Buffer> SBuffer;
} // kapusha