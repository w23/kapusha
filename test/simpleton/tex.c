#include <GLFW/glfw3.h>

#include "kapusha/simpleton.h"
#include "kapusha/render.h"

static KPrender_batch_o batch;
static KPrender_program_o program;

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

KPrender_program_env_o env;
KPrender_cmd_fill_t fill;
KPrender_cmd_rasterize_t raster;
KPmat4f proj;

void simpleton_init(int argc, const char *argv[]) {
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
  kpRenderProgramAttributeTag(program, "us2_tex", kpRenderTag("STEX"));

  kpRenderProgramArgumentTag(program, "um4_mvp", kpRenderTag("MMVP"));

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
  int x, y;
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

void simpleton_size(int width, int height) {
  KPrender_destination_t dest;
  kpRenderDestinationDefaults(&dest);
  dest.viewport.tr.x = width;
  dest.viewport.tr.y = height;
  kpRenderSetDestination(&dest);

  proj = kpMat4fProjPerspective(1.f, 100.f, (KPf32)width/(KPf32)height, 90.f);
}

void simpleton_draw(KPtime_ms pts) {
  KPdquatf q = kpDquatfRotationTranslation(
    kpVec3fNormalize(kpVec3f(0, 1, 1)), pts / 1000.f, kpVec3f(0, 0, -5.f-3.f*kpSinf(pts/1000.f)));
  KPmat4f m = kpMat4fMulm4(proj, kpMat4fdq(q));

  kpRenderProgramEnvSetMat4f(&env, kpRenderTag("MMVP"), &m);

  kpRenderExecuteCommand(&fill.header);
  kpRenderExecuteCommand(&raster.header);
}

static void glfw_resize_cb(GLFWwindow *win, int w, int h) {
  KP_UNUSED(win);
  simpleton_size(w, h);
}

int main(int argc, char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);

  if (!glfwInit())
    return -1;

  GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  glfwMakeContextCurrent(window);
  simpleton_init(argc, (const char**)argv);
  glfwSetWindowSizeCallback(window, glfw_resize_cb);

  while (!glfwWindowShouldClose(window)){
    simpleton_draw(glfwGetTime() * 1000.0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

}
