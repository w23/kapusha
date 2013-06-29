#pragma once
#include "array.h"

namespace kapusha {
  //! \brief naive u32->POD map
  class Map {
  public:
    Map(std::size_t item_size);

    const void *find(u32 key) const;
    inline void *find(u32 key) {
      return const_cast<void*>(find(key));
    }
    void *set(u32 key, const void* data);

  private:
    //! \brief trivial hash
    inline static u32 bucket(u32 key) {
      key ^= key >> 16;
      key ^= key >> 8;
      return key & 63;
    }
    std::size_t itemSize_;
    Array buckets_[64];
  }; // class Map

  template <typename T>
  class MapOf : public Map {
  public:
    MapOf() : Map(sizeof(T)) {}
    inline const T *find(u32 key) const {
      return reinterpret_cast<const T*>(Map::find(key));
    }
    inline T *find(u32 key) {
      return reinterpret_cast<T*>(Map::find(key));
    }
    inline T *set(u32 key, const T *value) {
      return reinterpret_cast<T*>(Map::set(key, value));
    }
  }; // class MapOf
} // namespace kapusha
