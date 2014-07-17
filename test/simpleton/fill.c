#include "kapusha/simpleton.h"
#include "kapusha/render.h"

void simpleton_init(int argc, const char *argv[]) {
}

void simpleton_size(int width, int height) {
  KPrender_destination_t dest;
  kpRenderDestinationDefaults(&dest);
  dest.viewport.tr.x = width;
  dest.viewport.tr.y = height;
  kpRenderSetDestination(&dest);
}

void simpleton_draw(KPtime_ms pts) {
  KPrender_cmd_fill_t fill;
  fill.header.cmd = KPrender_Command_Fill;
  fill.color = kpVec4f(
    ((pts>>4)&0xff)/255.f,
    ((pts>>8)&0xff)/255.f,
    ((pts>>12)&0xff)/255.f, 1.0f);
  kpRenderExecuteCommand(&fill.header);
}
