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
    Buffer(Binding binding_hint = BindingArray);
    ~Buffer();

    void load(Stream* data, unsigned size);
    void load(void* data, unsigned size);

    //! \todo protected:
    unsigned name() const { return name_; }
    
  private: // noncopyable
    Buffer& operator=(const Buffer& right) { return *this; }
    Buffer(const Buffer& right) {}

  private:
    void bind() const;

    Binding bindingHint_;
    unsigned name_;
  };
  
  typedef shared<Buffer> SBuffer;
} // kapusha
