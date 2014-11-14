#include <stddef.h>
#include <kapusha/window.h>
#include <kapusha/render.h>

static const char shader_vertex[] =
"uniform mat4 um4_mvp;\n"
"attribute vec3 av3_vertex;\n"
"attribute vec3 av3_color;\n"
"varying vec2 vv2_tc;\n"
"void main() {\n"
"  gl_Position = um4_mvp * vec4(av3_vertex, 1.);\n"
"  vv2_tc = av3_color.xz*.98;\n"
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

static KPrender_program_env_o env;
static KPrender_cmd_fill_t clear, frame_clear;
static KPrender_cmd_rasterize_t draw_cube;
static KPrender_sampler_o texture, frame_texture;
static KPrender_destination_t window, frame;
static KPmat4f proj;

static void create(const KPwindow_painter_header_t *create) {
  KP_UNUSED(create);
  KPrender_buffer_o buffer = kpRenderBufferCreate();
  KPblob_desc_t data;
  data.data = vertices;
  data.size = sizeof(vertices);
  kpRenderBufferUpload(buffer, KPRenderBufferFlagNone, data);

  KPrender_batch_o batch = kpRenderBatchCreate();
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

  KPrender_program_o program = kpRenderProgramCreate();

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

  clear.header.cmd = KPrender_Command_Fill;
  clear.color = kpVec4f(0, .5, 0, 0);

  frame_clear.header.cmd = KPrender_Command_Fill;
  frame_clear.color = kpVec4f(.5, 0, 0, 0);

  draw_cube.header.cmd = KPrender_Command_Rasterize;
  draw_cube.batch = batch;
  draw_cube.program = program;
  draw_cube.env_count = 1;
  draw_cube.env = &env;

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

  texture = kpRenderSamplerCreate();
  kpRenderSamplerUpload(texture, surface);
  kpRelease(surface);

  frame_texture = kpRenderSamplerCreate();
  kpRenderSamplerAlloc(frame_texture, 256, 256, KPSurfaceFormatU8RGBA);

  KPrender_framebuffer_params_t fbp;
  fbp.ncolors = 1;
  fbp.colors = &frame_texture;
  fbp.depth = 0;
  fbp.flags = KPRenderFramebufferFlagDepthAny;

  frame.framebuffer = kpRenderFramebufferCreate(&fbp);
  frame.viewport.tr.x = frame.viewport.tr.y
    = frame.viewport.bl.x = frame.viewport.bl.y = 0;
  frame.depth.test = KPRenderDepthTestEnabled;
  frame.depth.write = KPRenderDepthWriteEnabled;
  frame.depth.func = KPRenderDepthFuncLess;
}

static void configure(const KPwindow_painter_configure_t *cfg) {
  window.framebuffer = 0;
  window.viewport.bl.x = window.viewport.bl.y = 0;
  window.depth.test = KPRenderDepthTestEnabled;
  window.depth.write = KPRenderDepthWriteEnabled;
  window.depth.func = KPRenderDepthFuncLess;
  window.viewport.tr.x = cfg->width;
  window.viewport.tr.y = cfg->height;

  proj = kpMat4fMakePerspective(90.f, cfg->aspect, 1.f, 100.f);
}

static void paint(const KPwindow_painter_paint_t *paint) {
  const KPf32 pts = (paint->pts / 1000000ULL) / 1000.f;
  KPdquatf q = kpDquatfMakeTransform(
    kpVec3fNormalize(kpVec3f(0, 1, 1)), pts, kpVec3f(0, 0, -5.f-3.f*kpSinf(pts)));
  KPmat4f m = kpMat4fMul(kpMat4fMakePerspective(90.f, 1.f, 1.f, 100.f), kpMat4fMakeDquatf(q));

  kpRenderSetDestination(&frame);
  kpRenderExecuteCommand(&frame_clear.header);
  kpRenderProgramEnvSetSampler(env, kpRenderTag("STEX"), texture);
  kpRenderExecuteCommand(&draw_cube.header);

  m = kpMat4fMul(proj, kpMat4fMakeDquatf(q));
  kpRenderProgramEnvSetMat4f(env, kpRenderTag("MMVP"), &m);

  kpRenderSetDestination(&window);
  kpRenderExecuteCommand(&clear.header);
  kpRenderProgramEnvSetSampler(env, kpRenderTag("STEX"), frame_texture);
  kpRenderExecuteCommand(&draw_cube.header);
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

  wp.title = "kapusha sample: textured cube";
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
  KP_FAIL("Not implemented");
  return 0;
}
