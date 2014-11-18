#include "common/runner.h"
#include <kapusha/render.h>

static const KPvec2f vertices[3] = {
  {0.5f, -0.5f}, {0.0f, 0.5f}, {-0.5f, -0.5f}
};

static const char shader_vertex[] =
"attribute vec2 av2_vertex;\n"
"varying vec2 vv2_param;\n"
"void main() {\n"
"  gl_Position = vec4(av2_vertex, 0., 1.);\n"
"  vv2_param = vec2(.5) - av2_vertex;"
"}\n"
;

static const char shader_fragment[] =
"varying vec2 vv2_param;\n"
"uniform float uf_time;\n"
"void main() {\n"
"  gl_FragColor = vec4(vv2_param, .5+.5*sin(10.*(vv2_param.y+uf_time)), 1.);\n"
"}\n"
;

static KPrender_program_o program;
static KPrender_batch_o batch;
static KPrender_program_env_o env;

static void create() {
  KP_UNUSED(create);

  KPrender_buffer_o buf = kpRenderBufferCreate();
  KPblob_desc_t data;
  data.data = vertices;
  data.size = sizeof(vertices);
  kpRenderBufferUpload(buf, KPRenderBufferFlagNone, data);

  batch = kpRenderBatchCreate();
  KPrender_vertex_attrib_t vertex;
  vertex.buffer = buf;
  vertex.components = 2;
  vertex.offset = 0;
  vertex.stride = sizeof(KPvec2f);
  vertex.type = KPRenderVertexAttribF32;
  kpRenderBatchAttribSet(batch, kpRenderTag("VRTX"), &vertex);

  KPrender_draw_params_t draw;
  draw.buffer = 0;
  draw.count = 3;
  draw.offset = 0;
  draw.primitive = KPRenderDrawPrimitiveTriangleList;
  kpRenderBatchDrawSet(batch, &draw);

  program = kpRenderProgramCreate();

  data.data = shader_vertex;
  data.size = sizeof(shader_vertex);
  kpRenderProgramModuleSet(program, KPRenderProgramModuleVertex, data);

  data.data = shader_fragment;
  data.size = sizeof(shader_fragment);
  kpRenderProgramModuleSet(program, KPRenderProgramModuleFragment, data);

  kpRenderProgramArgumentTag(program, "uf_time", kpRenderTag("TIME"));
  kpRenderProgramAttributeTag(program, "av2_vertex", kpRenderTag("VRTX"));

  env = kpRenderProgramEnvCreate();
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
}

static void paint(const KPwindow_painter_event_t *event) {
  kpRenderProgramEnvSetScalarf(env, kpRenderTag("TIME"), event->time.pts / 1000000000.);

  KPrender_cmd_fill_t fill;
  fill.header.cmd = KPrender_Command_Fill;
  fill.color = kpVec4f(0.f, 0.f, 0.f, 1.f);
  kpRenderExecuteCommand(&fill.header);

  KPrender_cmd_rasterize_t raster;
  raster.header.cmd = KPrender_Command_Rasterize;
  raster.batch = batch;
  raster.program = program;
  raster.env_count = 1;
  raster.env = &env;
  kpRenderExecuteCommand(&raster.header);
}

static void destroy(const KPwindow_painter_event_t *event) {
  KP_UNUSED(event);
}

sample_t raster = {"kapusha: raster", create, configure, paint, destroy};