#include <kapusha/window.h>
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

static void create(const KPwindow_painter_header_t *create) {
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

static void configure(const KPwindow_painter_configure_t *cfg) {
  KPrender_destination_t dest;
  kpRenderDestinationDefaults(&dest);
  dest.viewport.tr.x = cfg->width;
  dest.viewport.tr.y = cfg->height;
  kpRenderSetDestination(&dest);
}

static void paint(const KPwindow_painter_paint_t *paint) {
  kpRenderProgramEnvSetScalarf(env, kpRenderTag("TIME"), paint->pts / 1000000000.);

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

static void destroy(const KPwindow_painter_header_t *destroy) {
  KP_UNUSED(destroy);
}

int kpuserAppCreate(int argc, const char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);

  KPwindow_params_t wp;

  KPuptr selector[] = {
    KPOutputSelectorType, KPOutputVideo,
    KPOutputSelector_End
  };
  KPsize outputs = kpOutputsSelect(selector, (KPoutput_o*)&wp.output, 1);
  KP_UNUSED(outputs);

  wp.title = "kapusha sample: raster";
  wp.user_data = 0;
  wp.painter_create_func = create;
  wp.painter_configure_func = configure;
  wp.painter_func = paint;
  wp.painter_destroy_func = destroy;
  wp.flags = 0;
  wp.width = wp.height = 0;
  kpWindowCreate(&wp);

  return 0;
}

int kpuserAppDestroy() {
  return 0;
}
