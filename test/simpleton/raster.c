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


void simpleton_init(int argc, const char *argv[]) {
  KPrender_buffer_o buf = kpRenderBufferCreate();
  KPblob_desc_t data;
  data.data = vertices;
  data.size = sizeof(vertices);
  kpRenderBufferUpload(buf, data, KPRenderBufferFlagNone);

  batch = kpRenderBatchCreate();
  KPrender_vertex_attrib_t vertex;
  vertex.buffer = buf;
  vertex.components = 2;
  vertex.offset = 0;
  vertex.stride = sizeof(KPvec2f);
  vertex.type = KPRenderVertexAttribF32;
  kpRenderBatchAttribSet(batch, kpRenderTag("VRTX"), &vertex);
  
  KPrender_draw_params_t draw;
  draw.buffer = NULL;
  draw.count = 3;
  draw.data = NULL;
  draw.offset = 0;
  draw.primitive = KPRenderDrawPrimitiveTriangleList;
  kpRenderBatchDrawSet(batch, &draw);
  
  program = kpRenderProgramCreate();
  KPrender_program_value_t value;
  value.type = KPRenderProgramValueScalarf;
  value.value.f[0] = 0.0f;
  kpRenderProgramArgumentSet(program, kpRenderTag("TIME"), "uf_time", value);
  kpRenderProgramAttributeSet(program, kpRenderTag("VRTX"), "av3_vertex");
  
  data.data = shader_vertex;
  data.size = sizeof(shader_vertex);
  kpRenderProgramModuleSet(program, KPRenderProgramModuleVertex, data);
  
  data.data = shader_fragment;
  data.size = sizeof(shader_fragment);
  kpRenderProgramModuleSet(program, KPRenderProgramModuleFragment, data);
}

void simpleton_size(int width, int height) {
  KPrender_destination_t dest;
  kpRenderDestinationDefaults(&dest);
  dest.viewport.tr.x = width;
  dest.viewport.tr.y = height;
  kpRenderSetDestination(&dest);
}

void simpleton_draw(KPtime_ms pts) {
  KPrender_program_env_t env;
  kpRenderProgramEnvClear(&env);
  KPrender_program_value_t value;
  value.type = KPRenderProgramValueScalarf;
  value.value.f[0] = pts / 1000.0f;
  kpRenderProgramEnvSet(&env, kpRenderTag("TIME"), &value);
  
  KPrender_cmd_rasterize_t raster;
  raster.header.cmd = KPrender_Command_Rasterize;
  raster.batch = batch;
  raster.program = program;
  raster.env_count = 1;
  raster.env = &env;
  kpRenderExecuteCommand(&raster.header);
}
