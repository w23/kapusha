#include <cstring>
#include "tagged_array.h"

namespace kapusha {
namespace ooo {

const tagged_array_t::item_header_t *tagged_array_t::find(tag_t tag) const {
  for (u32 i = 0; i < array_.size(); ++i) {
    const item_header_t *ihdr = array_.cast<item_header_t>(i);
    if (ihdr->tag == tag) return ihdr;
  }
  return nullptr;
}

void *tagged_array_t::set(tag_t tag, const void *source_item) {
  void *item = operator[](tag);
  if (item) {
    array_.item_dtor()(item);
  } else {
    item_header_t *ihdr = reinterpret_cast<item_header_t*>(array_.alloc_back());
    ihdr->tag = tag;
    item = ihdr->data;
  }
  if (source_item) memcpy(item, source_item, array_.item_size());
  return item;
}

const void *tagged_array_t::operator[](tag_t tag) const {
  const item_header_t *ihdr = find(tag);
  if (ihdr) return ihdr->data;
  return nullptr;
}

void *tagged_array_t::operator[](tag_t tag) {
  const item_header_t *ihdr = find(tag);
  if (ihdr) return const_cast<char*>(ihdr->data);
  return nullptr;
}

tagged_array_t::const_item_t tagged_array_t::operator[](u32 index) const {
  KP_ASSERT(index < array_.size());
  const item_header_t *ihdr = array_.cast<item_header_t>(index);
  return const_item_t(ihdr->tag, ihdr->data);
}

tagged_array_t::item_t tagged_array_t::operator[](u32 index) {
  KP_ASSERT(index < array_.size());
  item_header_t *ihdr = array_.get<item_header_t>(index);
  return item_t(ihdr->tag, ihdr->data);
}

} // namespace ooo
} // namespace kapusha
