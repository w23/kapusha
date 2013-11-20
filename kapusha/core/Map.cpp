#include "Map.h"

namespace kapusha {
  Map::Map(u32 item_size) : item_size_(item_size) {
    for (int i = 0; i < 64; ++i) buckets_[i].init(item_size_ + 4);
  }

  const void *Map::find(u32 key) const {
    const Array &b = buckets_[bucket(key)];
    for (u32 i = 0; i < b.size(); ++i) {
      const void *item = b.get(i);
      if (key == *reinterpret_cast<const u32*>(item)) {
        return reinterpret_cast<const u32*>(item) + 1;
      }
    }
    return nullptr;
  }

  void* Map::set(u32 key, const void* data) {
    Array &b = buckets_[bucket(key)];
    for (u32 i = 0; i < b.size(); ++i) {
      void *item = b.get(i);
      if (key == *reinterpret_cast<u32*>(item)) {
        void *item_data = reinterpret_cast<u32*>(item) + 1;
        if (data)
          memcpy(item_data, data, item_size_);
        return item_data;
      }
    }
    u32 *item = reinterpret_cast<u32*>(b.alloc_back(1));
    *item = key;
    if (data) memcpy(item + 1, data, item_size_);
    return item + 1;
  }
} // namespace kapusha
