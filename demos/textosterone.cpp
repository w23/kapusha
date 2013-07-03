#include <kapusha/viewport.h>
#include <kapusha/render.h>
#include <kapusha/fontain.h>

#include <kapusha/fontain/harftype/Freetype.h>

using namespace kapusha;

class Viewport : public IViewport {
public:
  Viewport() : face_(fontain.getFace("/usr/share/fonts/liberation-fonts/LiberationMono-Regular.ttf", vec2i(23))) {}
  //Viewport() : face_(fontain.getFace("/usr/share/fonts/liberation-fonts/LiberationSerif-Italic.ttf", vec2i(32))) {}
  ~Viewport() { delete face_; }
  void init(IViewportController* ctrl, Context *context);
  void resize(vec2i size);
  void draw(int ms, float dt);
  void close();
private:
  fontain::harftype::Freetype fontain;
  IViewportController *ctrl_;
  Context *context_;
  SBatch textAtlas_;
  fontain::IFace *face_;
};
void Viewport::init(IViewportController *ctrl, Context *context) {
  ctrl_ = ctrl;
  context_ = context;

  static const char *shader_vertex =
    "attribute vec4 av4_vertex;\n"
    "attribute vec2 av2_tex;\n"
    "varying "
#if KAPUSHA_GLES
    "lowp "
#endif
    "vec2 vv2_vertex;\n"
    "void main(){\n"
      "gl_Position = av4_vertex;\n"
      "vv2_vertex = av2_tex;\n"
    "}";

  static const char *shader_fragment =
    "uniform sampler2D us2_atlas;\n"
    "uniform sampler2D us2_test;\n"
    "varying "
#if KAPUSHA_GLES
    "lowp "
#endif
    "vec2 vv2_vertex;\n"
    "void main(){\n"
      "gl_FragColor = vec4(texture2D(us2_atlas, vv2_vertex).a);\n"
    "}";

  static const char *test_string = 
    "WAVAfl"
    "В чащах юга жил бы цитрус? Да, но фальшивый экземпляр!"
    "/usr/share/fonts/liberation-fonts/LiberationMono-Regular.ttf"
  ;

  fontain::String *string = face_->createString(context_, test_string);

  const u32 nvertices = string->length() * 4 + 4;
  struct vertex_t {
    vec2f pos, tex;
  } *vertices = new vertex_t[nvertices];

  u32 nindices = string->length() * 6 + 6;
  u16 *indices = new u16[nindices];

  vec2f offset(-1.f, 0.f);
  vec2f scale = vec2f(string->atlas()->meta().size).recip();
  vec2f pscale(.002f);
  vertex_t *v = vertices;
  for (u32 i = 0; i < string->length(); ++i, v += 4) {
    const fontain::String::Glyph &g = (*string)[i];
    const rect2i &r = g.rectInAtlas;
    v[0].pos = offset + vec2f(g.offset + vec2i(0, r.height())) * pscale;
    v[1].pos = offset + vec2f(g.offset) * pscale;
    v[2].pos = offset + vec2f(g.offset + r.size()) * pscale;
    v[3].pos = offset + vec2f(g.offset + vec2i(r.width(), 0)) * pscale;
    v[0].tex = vec2f(r.topLeft()) * scale;
    v[1].tex = vec2f(r.bottomLeft()) * scale;
    v[2].tex = vec2f(r.topRight()) * scale;
    v[3].tex = vec2f(r.bottomRight()) * scale;
    indices[i*6 + 0] = i*4 + 0;
    indices[i*6 + 1] = i*4 + 1;
    indices[i*6 + 2] = i*4 + 2;
    indices[i*6 + 3] = i*4 + 3;
    indices[i*6 + 4] = i*4 + 2;
    indices[i*6 + 5] = i*4 + 1;
  }
  {
  int i = string->length();
  v[0].pos = vec2f(-1.f,  1.f);
  v[1].pos = vec2f(-1.f, -1.f);
  v[2].pos = vec2f( 1.f,  1.f);
  v[3].pos = vec2f( 1.f, -1.f);
  v[0].tex = vec2f(0.f, 1.f);
  v[1].tex = vec2f(0.f, 0.f);
  v[2].tex = vec2f(1.f, 1.f);
  v[3].tex = vec2f(1.f, 0.f);
  indices[i*6 + 0] = i*4 + 0;
  indices[i*6 + 1] = i*4 + 1;
  indices[i*6 + 2] = i*4 + 2;
  indices[i*6 + 3] = i*4 + 3;
  indices[i*6 + 4] = i*4 + 2;
  indices[i*6 + 5] = i*4 + 1;
  }

  Buffer *buffer = new Buffer(Buffer::BindingArray);
  buffer->load(context_, vertices, sizeof(vertex_t) * nvertices);
  
  Buffer *iuffer = new Buffer(Buffer::BindingIndex);
  iuffer->load(context_, indices, 2 * nindices);
    
  Program *program = new Program(shader_vertex, shader_fragment);
  Material *material = new Material(program);
  material->setUniform("us2_atlas", string->atlas());
  material->blend().enable();
  material->blend().setFunction(BlendState::ConstOne, BlendState::OneMinusSourceAlpha);
  Batch* batch = new Batch();
  batch->setMaterial(material);
  batch->setAttribSource("av4_vertex", buffer, 2,  offsetof(vertex_t, pos), sizeof(vertex_t));
  batch->setAttribSource("av2_tex", buffer, 2,  offsetof(vertex_t, tex), sizeof(vertex_t));
  batch->setGeometry(Batch::GeometryTriangleList, 0, nindices, Batch::IndexU16, iuffer);
  textAtlas_ = batch;
  delete[] indices;
  delete[] vertices;
  delete string;
}
void Viewport::close() {
  textAtlas_.reset();
}
void Viewport::resize(vec2i size) {
  glViewport(0, 0, size.x, size.y);
}
void Viewport::draw(int ms, float dt) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_ASSERT
  textAtlas_->draw(context_);
}
IViewport *makeViewport() {
  return new Viewport();
}
