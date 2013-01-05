#pragma once
#include "../core/Shared.h"

namespace kapusha {

  class Stream;
  
  class Buffer : public Shared {
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
    
    void* map(unsigned size);
    void unmap();
    
    void bind(Binding binding = BindingArray) const;

  private: // noncopyable
    Buffer& operator=(const Buffer& right) { return *this; }
    Buffer(const Buffer& right) {}

  private:
    unsigned name_;
  };
  
  typedef shared<Buffer> SBuffer;
} // kapusha