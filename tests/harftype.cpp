#include <kapusha/core.h>
#include <kapusha/fontain/harftype/Freetype.h>
#include <kapusha/fontain/IString.h>

namespace kapusha {
void log::sys_write(const char *message) {
  fprintf(stderr, "%s\n", message);
}
}

using namespace kapusha;
using namespace kapusha::fontain;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    L("more args!");
    return 1;
  }
  harftype::Freetype ft;
  SFace face = ft.getFace(argv[1], vec2i(23));
  IString *str = face->createEmptyString();
  const IString::Layout* l = str->setStringAndCalculate(argv[2]);
  L("Text size (%d, %d) - (%d, %d), %d glyphs", l->box.min.x, l->box.min.y,
    l->box.max.x, l->box.max.y, l->nglyphs);
  for (unsigned i = 0; i < l->nglyphs; ++i) {
    const IString::Glyph *g = l->glyphs + i;
    L("%d: %d-%d, (%d, %d) - (%d, %d)", i, g->start, g->end,
      g->box.min.x, g->box.min.y, g->box.max.x, g->box.max.y);
  }
  return 0;
}
