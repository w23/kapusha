#pragma once

namespace kapusha {

  class Stream;

  class Buffer {
  public:
    Buffer();
    ~Buffer();

    void load(Stream* data, unsigned size);

  public:
    void bindAttribute(int index, int components, int offset, int stride) const;

  private:
    unsigned buffer_name_;
  };

} // kapusha