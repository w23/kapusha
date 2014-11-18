#include <stddef.h>
#include "common/runner.h"
#include <kapusha/render.h>

static const char shader_vertex[] =
"uniform mat4 um4_mvp;\n"
"attribute vec3 av3_vertex;\n"
"attribute vec3 av3_color;\n"
"varying vec2 vv2_tc;\n"
"void main() {\n"
"  gl_Position = um4_mvp * vec4(av3_vertex, 1.);\n"
"  vv2_tc = av3_color.xy * 1.6 - vec2(.8);\n"
"}\n"
;

static const char shader_fragment[] =
"uniform sampler2D us2_tex;\n"
"varying vec2 vv2_tc;\n"
"void main() {\n"
"  gl_FragColor = texture2D(us2_tex,vv2_tc);\n"
"}\n"
;

typedef struct {
  KPvec3f vertex;
  KPvec4b color;
} vertex_t;

static vertex_t vertices[8] = {
  {{ .5f, -.5f,  .5f}, {255,   0, 255, 0}},
  {{ .5f,  .5f,  .5f}, {255, 255, 255, 0}},
  {{-.5f, -.5f,  .5f}, {  0,   0, 255, 0}},
  {{-.5f,  .5f,  .5f}, {  0, 255, 255, 0}},
  {{-.5f, -.5f, -.5f}, {  0,   0,   0, 0}},
  {{-.5f,  .5f, -.5f}, {  0, 255,   0, 0}},
  {{ .5f, -.5f, -.5f}, {255,   0,   0, 0}},
  {{ .5f,  .5f, -.5f}, {255, 255,   0, 0}}
};

static KPu16 indices[36] = {
  0, 1, 3, 0, 3, 2,
  2, 3, 5, 2, 5, 4,
  4, 5, 7, 4, 7, 6,
  6, 7, 1, 6, 1, 0,
  1, 7, 5, 1, 5, 3,
  0, 2, 4, 0, 4, 6
};


static KPrender_batch_o batch;
static KPrender_program_o program;
static KPrender_program_env_o env;
static KPrender_cmd_fill_t fill;
static KPrender_cmd_rasterize_t raster;
static KPmat4f proj;

static void create() {
  KP_UNUSED(create);
  KPrender_buffer_o buffer = kpRenderBufferCreate();
  KPblob_desc_t data;
  data.data = vertices;
  data.size = sizeof(vertices);
  kpRenderBufferUpload(buffer, KPRenderBufferFlagNone, data);

  batch = kpRenderBatchCreate();
  KPrender_vertex_attrib_t attrib;
  attrib.buffer = buffer;
  attrib.components = 3;
  attrib.offset = offsetof(vertex_t, vertex);
  attrib.stride = sizeof(vertex_t);
  attrib.type = KPRenderVertexAttribF32;
  kpRenderBatchAttribSet(batch, kpRenderTag("VRTX"), &attrib);
  kpRelease(buffer);

  attrib.buffer = buffer;
  attrib.components = 3;
  attrib.offset = offsetof(vertex_t, color);
  attrib.stride = sizeof(vertex_t);
  attrib.type = KPRenderVertexAttribU8;
  kpRenderBatchAttribSet(batch, kpRenderTag("COLR"), &attrib);

  buffer = kpRenderBufferCreate();
  data.data = indices;
  data.size = sizeof(indices);
  kpRenderBufferUpload(buffer, KPRenderBufferFlagElement, data);

  KPrender_draw_params_t draw;
  draw.buffer = buffer;
  draw.index_type = KPRenderDrawIndexU16;
  draw.count = 36;
  draw.offset = 0;
  draw.primitive = KPRenderDrawPrimitiveTriangleList;
  kpRenderBatchDrawSet(batch, &draw);
  kpRelease(buffer);

  program = kpRenderProgramCreate();

  data.data = shader_vertex;
  data.size = sizeof(shader_vertex);
  kpRenderProgramModuleSet(program, KPRenderProgramModuleVertex, data);

  data.data = shader_fragment;
  data.size = sizeof(shader_fragment);
  kpRenderProgramModuleSet(program, KPRenderProgramModuleFragment, data);

  kpRenderProgramAttributeTag(program, "av3_vertex", kpRenderTag("VRTX"));
  kpRenderProgramAttributeTag(program, "av3_color", kpRenderTag("COLR"));

  kpRenderProgramArgumentTag(program, "um4_mvp", kpRenderTag("MMVP"));
  kpRenderProgramArgumentTag(program, "us2_tex", kpRenderTag("STEX"));

  env = kpRenderProgramEnvCreate();

  fill.header.cmd = KPrender_Command_Fill;
  fill.color = kpVec4f(0, .5, 0, 0);

  raster.header.cmd = KPrender_Command_Rasterize;
  raster.batch = batch;
  raster.program = program;
  raster.env_count = 1;
  raster.env = &env;

  KPsurface_o surface = kpSurfaceCreate(256, 256, KPSurfaceFormatU8RGBA);
  KPu32 *pix = (KPu32*)surface->buffer;
  KPu32 x, y;
  for (y = 0; y < surface->height; ++y)
    for (x = 0; x < surface->width; ++x, ++pix) {
      KPu8 r = x + y;
      KPu8 g = x ^ y;
      KPu8 b = x * y;
      *pix = (0xff000000) | (b << 16) | (g << 8) | r;
    }

  KPrender_sampler_o sampler = kpRenderSamplerCreate();
  kpRenderSamplerUpload(sampler, surface);
  kpRelease(surface);

  kpRenderProgramEnvSetSampler(env, kpRenderTag("STEX"), sampler);
  kpRelease(sampler);
}

static void configure(const KPwindow_painter_event_t *event) {
  KPrender_destination_t dest;
  dest.framebuffer = 0;
  dest.viewport.bl.x = dest.viewport.bl.y = 0;
  dest.depth.test = KPRenderDepthTestEnabled;
  dest.depth.write = KPRenderDepthWriteEnabled;
  dest.depth.func = KPRenderDepthFuncLess;
  dest.viewport.tr.x = event->configuration.width;
  dest.viewport.tr.y = event->configuration.height;
  kpRenderSetDestination(&dest);

  proj = kpMat4fMakePerspective(90.f, event->configuration.aspect, 1.f, 100.f);
}

static void paint(const KPwindow_painter_event_t *event) {
  const KPf32 pts = (event->time.pts / 1000000ULL) / 1000.f;
  KPdquatf q = kpDquatfMakeTransform(
    kpVec3fNormalize(kpVec3f(0, 1, 1)), pts, kpVec3f(0, 0, -5.f-3.f*kpSinf(pts)));
  KPmat4f m = kpMat4fMul(proj, kpMat4fMakeDquatf(q));

  kpRenderProgramEnvSetMat4f(env, kpRenderTag("MMVP"), &m);

  kpRenderExecuteCommand(&fill.header);
  kpRenderExecuteCommand(&raster.header);
}

static void destroy() {
  KP_UNUSED(destroy);
}

sample_t tex = {"kapusha: tex", create, configure, paint, destroy};