#include <GLFW/glfw3.h>

#include "kapusha/simpleton.h"
#include "kapusha/render.h"
#include "kapusha/math.h"

static KPrender_batch_o batch;
static KPrender_program_o program;

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
"  gl_FragColor = vec4(vv2_param, .5+.5*sin(10.*vv2_param.y+uf_time), 1.);\n"
"}\n"
;

KPrender_program_env_o env;

void simpleton_init(int argc, const char *argv[]) {
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

void simpleton_size(int width, int height) {
  KPrender_destination_t dest;
  kpRenderDestinationDefaults(&dest);
  dest.viewport.tr.x = width;
  dest.viewport.tr.y = height;
  kpRenderSetDestination(&dest);
}

void simpleton_draw(KPtime_ms pts) {
  kpRenderProgramEnvSetScalarf(env, kpRenderTag("TIME"), pts / 1000.0f);

  KPrender_cmd_rasterize_t raster;
  raster.header.cmd = KPrender_Command_Rasterize;
  raster.batch = batch;
  raster.program = program;
  raster.env_count = 1;
  raster.env = &env;
  kpRenderExecuteCommand(&raster.header);
}

int main(int argc, char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);

  if (!glfwInit())
    return -1;

  GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  glfwMakeContextCurrent(window);
  simpleton_init(argc, (const char**)argv);
  simpleton_size(640, 480);

  while (!glfwWindowShouldClose(window)){
    simpleton_draw(glfwGetTime() * 1000.0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

}
