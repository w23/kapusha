#include <limits.h>
#include "../render/gl.h"

#define KP__SYS "GL"

void kp__GlAssert(const char *file, int line) {
  int err = glGetError();
  if (err == 0) return;
  const char *errname;
  switch (err) {
    case GL_INVALID_ENUM: errname = "GL_INVALID_ENUM"; break;
    case GL_INVALID_VALUE: errname = "GL_INVALID_VALUE"; break;
    case GL_INVALID_OPERATION: errname = "GL_INVALID_OPERATION"; break;
    case GL_OUT_OF_MEMORY: errname = "GL_OUT_OF_MEMORY"; break;
    default: errname = "UNKNOWN";
  }
  KP__L("error %s[%d] @ %s:%d", errname, err, file, line);
  //kpSysExit(-1);
}

/******************************************************************************/
/* state */

#undef KP__SYS
#define KP__SYS "GL::state"

#define DECLARE_STATE \
  KP__render_state_t *state = kp__CurrentRenderState(); KP_ASSERT(state)

inline static KP__render_state_t *kp__CurrentRenderState() {
  /* \todo multithreading */
  static __thread KP__render_state_t state;
  return &state;
}

void kp__RenderInitThread() {
  DECLARE_STATE;
  kpMemset(state, 0, sizeof(*state));
}

void kp__RenderCloseThread() {
  DECLARE_STATE;
  for(KPu32 i = 0; i < KP__RenderBufferTarget_MAX; ++i)
    kpRelease(state->buffer_targets[i]);
  for(KPu32 i = 0; i < KP__RenderSamplers_MAX; ++i)
    kpRelease(state->sampler_units[i]);
  kpRelease(state->framebuffer);
}

/*
static void kp__RenderStateInit(KP__render_state_t *state) {
  int i = 0;
  for (i = 0; i < KP__RenderBufferTarget_MAX; ++i)
    state->buffer_targets[i] = 0;
  for (i = 0; i < KP__RenderSamplers_MAX; ++i)
    state->sampler_units[i] = 0;
  state->sampler_unit_active = 0;
  state->sampler_group_unit = 0;
  state->program = 0;
}
*/

/******************************************************************************/
/* buffer */

#undef KP__SYS
#define KP__SYS "GL::buffer"

static GLenum KP__render_buffer_target_gl[KP__RenderBufferTarget_MAX] = {
  GL_ARRAY_BUFFER,
  GL_ELEMENT_ARRAY_BUFFER
};

static void kp__RenderStateBufferBind(
  KP__render_state_t *state,
  KP__render_buffer_t *this, KP__RenderBufferTarget target)
{
  KP_ASSERT(target < KP__RenderBufferTarget_MAX);
  if (state->buffer_targets[target] != this) {
    glBindBuffer(KP__render_buffer_target_gl[target], this?this->name:0); KP__GLASSERT
  }
  state->buffer_targets[target] = this;
}

static void kp__RenderStateBufferUnbind(
  KP__render_state_t *state,
  KP__render_buffer_t *this)
{
  KP_ASSERT(this);
  int i;
  for (i = 0; i < KP__RenderBufferTarget_MAX; ++i)
    if (this == state->buffer_targets[i]) {
      glBindBuffer(KP__render_buffer_target_gl[i], 0); KP__GLASSERT
      state->buffer_targets[i] = 0;
    }
}

static void kp__RenderBufferDtor(void *buffer) {
  DECLARE_STATE;
  KP_THIS(KP__render_buffer_t, buffer);
  kp__RenderStateBufferUnbind(state, this);
  KP__L("%p: delete %d", this, this->name);
  glDeleteBuffers(1, &this->name); KP__GLASSERT
}

KPrender_buffer_o kpRenderBufferCreate() {
  DECLARE_STATE;
  KP__render_buffer_t *this = KP_NEW(KP__render_buffer_t);
  this->O.dtor = kp__RenderBufferDtor;
  glGenBuffers(1, &this->name); KP__GLASSERT
  KP_ASSERT(this->name != 0);
  KP__L("%p: create %d", this, this->name);
  return this;
}

void kpRenderBufferUpload(KPrender_buffer_o buffer,
  KPu32 flags,
  KPblob_desc_t data)
{
  DECLARE_STATE;
  KP_THIS(KP__render_buffer_t, buffer);
  KP__RenderBufferTarget target = (flags & KPRenderBufferFlagElement) ?
    KP__RenderBufferTargetElementArray : KP__RenderBufferTargetArray;
  kp__RenderStateBufferBind(state, this, target);
  KP__L("%p: upload %d", this, (int)data.size);
  glBufferData(KP__render_buffer_target_gl[target], data.size, data.data,
    (flags & KPRenderBufferFlagDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  KP__GLASSERT

}

/******************************************************************************/
/* sampler */

#undef KP__SYS
#define KP__SYS "GL::sampler"

static void kp__RenderStateSamplerGroupBegin(KP__render_state_t *state) {
  KP_ASSERT(state->sampler_group_unit == 0);
  state->sampler_group_unit = 1;
}

static void kp__RenderStateSamplerGroupEnd(KP__render_state_t *state) {
  KP_ASSERT(state->sampler_group_unit > 0);
  state->sampler_group_unit = 0;
}

static int kp__RenderStateSamplerBind(KP__render_state_t *state,
  KP__render_sampler_t *this)
{
  int unit;
  if (state->sampler_group_unit > 0) {
    unit = state->sampler_group_unit - 1;
    if (unit == KP__RenderSamplers_MAX)
      return -1;

    if (unit != state->sampler_unit_active) {
      glActiveTexture(GL_TEXTURE0 + unit); KP__GLASSERT
      state->sampler_unit_active = unit;
    }

    ++state->sampler_group_unit;
  } else
    unit = state->sampler_unit_active;

  if (state->sampler_units[unit] == this)
    return unit;

  /*KP__L("%p: bind %d to unit %d", this, this->name, unit);*/
  glBindTexture(GL_TEXTURE_2D, this->name); KP__GLASSERT
  state->sampler_units[unit] = this;
  return unit;
}

void kp__RenderStateSamplerUnbind(KP__render_state_t *state,
  KP__render_sampler_t *sampler)
{
  int i;
  for (i = 0; i < KP__RenderSamplers_MAX; ++i) {
    if (state->sampler_units[i] == sampler) {
      if (state->sampler_unit_active != i) {
        glActiveTexture(GL_TEXTURE0 + i); KP__GLASSERT
        state->sampler_unit_active = i;
      }
      glBindTexture(GL_TEXTURE_2D, 0); KP__GLASSERT
      state->sampler_units[i] = 0;
    }
  }
}

KPrender_sampler_o kpRenderSamplerCreate() {
  DECLARE_STATE;
  KP__render_sampler_t *this = KP_NEW(KP__render_sampler_t);
  this->name = 0;
  glGenTextures(1, &this->name); KP__GLASSERT
  KP__L("%p: create texture %d", this, this->name);
  this->O.dtor = kp__RenderSamplerDtor;
  return this;
}

static void kp__RenderSamplerDtor(void *s) {
  DECLARE_STATE;
  KP_THIS(KP__render_sampler_t, s);
  KP__L("%p: destroy texture %d", this, this->name);
  kp__RenderStateSamplerUnbind(state, this);
  glDeleteTextures(1, &this->name); KP__GLASSERT
}

int kp__RenderSamplerUpload(KP__render_sampler_o this,
  KPu32 width, KPu32 height, KPSurfaceFormat format, const void *data)
{
  DECLARE_STATE;
  GLuint fmt_internal, fmt, type;
  switch (format) {
    case KPSurfaceFormatU8R:
      type = GL_UNSIGNED_BYTE; fmt_internal = GL_ALPHA; fmt = GL_ALPHA; break;
    case KPSurfaceFormatU8RG:
      type = GL_UNSIGNED_BYTE;
      fmt_internal = GL_LUMINANCE_ALPHA; fmt = GL_LUMINANCE_ALPHA; break;
    case KPSurfaceFormatU8RGB:
      type = GL_UNSIGNED_BYTE; fmt_internal = GL_RGB; fmt = GL_RGB; break;
    case KPSurfaceFormatU8RGBA:
      type = GL_UNSIGNED_BYTE; fmt_internal = GL_RGBA; fmt = GL_RGBA; break;
    case KPSurfaceFormatR5G6B5:
      type = GL_UNSIGNED_SHORT_5_6_5; fmt_internal = GL_RGB; fmt = GL_RGB;
      break;
    case KPSurfaceFormatF32RGBA:
      type = GL_FLOAT; fmt_internal = GL_RGBA32F; fmt = GL_RGBA; break;
    default:
      KP_FAIL("Invalid surface format %d", format);
      return 0;
  }
  kp__RenderStateSamplerBind(state, this);
  glTexImage2D(GL_TEXTURE_2D, 0, fmt_internal, width, height, 0, fmt, type, data);
  KP__GLASSERT

  this->width = width;
  this->height = height;

  /* \todo this should be program-specific */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); KP__GLASSERT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); KP__GLASSERT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); KP__GLASSERT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); KP__GLASSERT
  return 1;
}

int kpRenderSamplerAlloc(KPrender_sampler_o sampler,
  KPu32 width, KPu32 height, KPSurfaceFormat format)
{
  return kp__RenderSamplerUpload(sampler,
    width, height, format, 0);
}

int kpRenderSamplerUpload(KPrender_sampler_o sampler, KPsurface_o surface) {
  return kp__RenderSamplerUpload(sampler,
    surface->width, surface->height, surface->format, surface->buffer);
}

/******************************************************************************/
/* program environment */

#undef KP__SYS
#define KP__SYS "GL::env"

void kp__RenderProgramEnvDtor(void *env) {
  KP_THIS(KP__render_program_env_t, env);
  int i;
  for (i = 0; i < KP__RENDER_PROGRAM_ENV_MAX_VALUES; ++i)
    if (this->values[i].type == KP__RenderProgramEnvValueSampler)
      kpRelease(this->values[i].v.sampler);
}

KPrender_program_env_o kpRenderProgramEnvCreate() {
  DECLARE_STATE;
  KP__render_program_env_t *this = KP_NEW(KP__render_program_env_t);
  KP__L("%p created", this);
  this->O.dtor = kp__RenderProgramEnvDtor;
  kpMemset(this->values, 0, sizeof(this->values));
  return this;
}

int kp__RenderProgramEnvFindSlot(
  KP__render_program_env_t *this,
  KPrender_tag_t tag)
{
  int i;
  for (i = 0; i < KP__RENDER_PROGRAM_ENV_MAX_VALUES; ++i) {
    /*KP__L("%p set %c%c%c%c(%d)", this,
      this->values[i].tag.tag.name[0],this->values[i].tag.tag.name[1],this->values[i].tag.tag.name[2],this->values[i].tag.tag.name[3],
      i);*/
    if (this->values[i].tag.tag.value == 0
        || this->values[i].tag.tag.value == tag.tag.value)
      return i;
  }
  return -1;
}

int kp__RenderProgramEnvSetNScalar(
  KP__render_program_env_t *this,
  KPrender_tag_t tag,
  KP__RenderProgramEnvValueType type,
  const KPf32 *value,
  int n)
{
  KP_ASSERT(n > 0);
  KP_ASSERT(n <= 16);
  int slot = kp__RenderProgramEnvFindSlot(this, tag);
  /*KP__L("%p set %c%c%c%c(%d)", this,
    tag.tag.name[0],tag.tag.name[1],tag.tag.name[2],tag.tag.name[3],
    slot);*/
  if (slot < 0) return 0;
  this->values[slot].tag = tag;
  this->values[slot].type = type;
  kpMemcpy(&this->values[slot].v.f, value, sizeof(KPf32) * n);
  return 1;
}

int kpRenderProgramEnvSetScalarf(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  KPf32 value)
{
  KP_THIS(KP__render_program_env_t, env);
  /*KP__L("%p set %c%c%c%c(%d) = %f", this,
    tag.tag.name[0],tag.tag.name[1],tag.tag.name[2],tag.tag.name[3], value);*/
  return kp__RenderProgramEnvSetNScalar(this, tag,
    KP__RenderProgramEnvValueScalarf, &value, 1);
}

int kpRenderProgramEnvSetVec4f(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  const KPvec4f *value)
{
  KP_THIS(KP__render_program_env_t, env);
  return kp__RenderProgramEnvSetNScalar(this, tag,
    KP__RenderProgramEnvValueVec4f, &value->x, 4);
}

int kpRenderProgramEnvSetMat4f(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  const KPmat4f *value)
{
  KP_THIS(KP__render_program_env_t, env);
  KPmat4f tr = kpMat4fTranspose(*value);
  return kp__RenderProgramEnvSetNScalar(this, tag,
    KP__RenderProgramEnvValueMat4f, &tr.r[0].x, 16);
}

int kpRenderProgramEnvSetSampler(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  KPrender_sampler_o sampler)
{
  KP_THIS(KP__render_program_env_t, env);
  int slot = kp__RenderProgramEnvFindSlot(this, tag);
  if (slot < 0) return 0;
  /*KP__L("%p set %c%c%c%c(%d) = %p", this,
    tag.tag.name[0],tag.tag.name[1],tag.tag.name[2],tag.tag.name[3],
    slot,
    sampler);*/
  this->values[slot].tag = tag;
  if (this->values[slot].type == KP__RenderProgramEnvValueSampler)
    kpRelease(this->values[slot].v.sampler);
  this->values[slot].type = KP__RenderProgramEnvValueSampler;
  kpRetain(sampler);
  this->values[slot].v.sampler = sampler;
  return 1;
}

static void kp__RenderProgramEnvApply(
  KP__render_state_t *state,
  KP__render_program_env_t *this,
  int index,
  int location)
{
  KP_ASSERT(this);
  KP_ASSERT(index < KP__RENDER_PROGRAM_ENV_MAX_VALUES);
  KP_ASSERT(index >= 0);
  KP_ASSERT(location >= 0);
  struct KP__render_program_env_value_t *value = this->values + index;
  switch (value->type) {
    case KP__RenderProgramEnvValueScalarf:
      glUniform1f(location, value->v.f[0]); KP__GLASSERT
      break;
    case KP__RenderProgramEnvValueVec4f:
      glUniform4f(location,
        value->v.f[0],
        value->v.f[1],
        value->v.f[2],
        value->v.f[3]); KP__GLASSERT
      break;
    case KP__RenderProgramEnvValueMat4f:
      glUniformMatrix4fv(location, 1, GL_FALSE, value->v.f); KP__GLASSERT
      break;
    case KP__RenderProgramEnvValueSampler:
      {
        int unit = kp__RenderStateSamplerBind(state, value->v.sampler);
        /*KP__L("%p: bind texture %p to %d", this, value->v.sampler, unit);*/
        KP_ASSERT(unit >= 0);
        glUniform1i(location, unit); KP__GLASSERT
      }
      break;
    default:
      KP_FAIL("Impossible KP__RenderProgramEnvValueType %d", value->type);
  }
}

/******************************************************************************/
/* program */

#undef KP__SYS
#define KP__SYS "GL::program"

static void kp__RenderProgramDtor(void *buffer) {
  KP_THIS(KP__render_program_t, buffer);
  glDeleteProgram(this->name); KP__GLASSERT
}

KPrender_program_o kpRenderProgramCreate() {
  DECLARE_STATE;
  KP__render_program_t *this = KP_NEW(KP__render_program_t);
  this->O.dtor = kp__RenderProgramDtor;
  this->name = glCreateProgram(); KP__GLASSERT
  KP__L("%p: create %d", this, this->name);
  int i;
  for (i = 0; i < KP__RenderProgramShader_MAX; ++i) this->shaders[i] = 0;
  for (i = 0; i < KP__RENDER_PROGRAM_MAX_ARGS; ++i)
    this->args[i].tag.tag.value = 0;
  for (i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i)
    this->attribs[i].tag.tag.value = 0;
  return this;
}

int kpRenderProgramModuleSet(
  KPrender_program_o program,
  KPRenderProgramModuleType module,
  KPblob_desc_t data)
{
  KP_THIS(KP__render_program_t, program);
  KP_ASSERT(data.size < 0xfffffffful);
  KPu32 flag;
  GLint shader;
  switch (module) {
  case KPRenderProgramModuleVertex:
    flag = KP__RenderProgramHaveVertex;
    if (this->shaders[KP__RenderProgramShaderVertex] == 0) {
      this->shaders[KP__RenderProgramShaderVertex] =
        glCreateShader(GL_VERTEX_SHADER); KP__GLASSERT
      KP__L("%p create vertex shader %d",
        this, this->shaders[KP__RenderProgramShaderVertex]);
    }
    shader = this->shaders[KP__RenderProgramShaderVertex];
    break;
  case KPRenderProgramModuleFragment:
    flag = KP__RenderProgramHaveFragment;
    if (this->shaders[KP__RenderProgramShaderFragment] == 0) {
      this->shaders[KP__RenderProgramShaderFragment] =
        glCreateShader(GL_FRAGMENT_SHADER); KP__GLASSERT
      KP__L("%p create fragment shader %d",
        this, this->shaders[KP__RenderProgramShaderFragment]);
    }
    shader = this->shaders[KP__RenderProgramShaderFragment];
    break;
  default:
    KP_FAIL("Invalid KPRenderProgramModule %d", module);
    return 0;
  }
  GLint length = (GLint)data.size;
  KP__L("%p glShaderSource(%d, 1, %p, %d)", this, shader, data.data, length);
  glShaderSource(shader, 1, (const GLchar**)&(data.data), &length); KP__GLASSERT
  glCompileShader(shader); KP__GLASSERT

  {
    GLint val;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &val);
    if (val != GL_TRUE) {
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &val);
      char *buffer = kpAlloc(val + 1);
      glGetShaderInfoLog(shader, val + 1, NULL, buffer);
      KP__L("%p: %s", this, buffer);
      kpFree(buffer);
      glDeleteShader(shader);
      return 0;
    }
  }

  glAttachShader(this->name, shader); KP__GLASSERT
  this->flags |= flag;
  if ((this->flags & KP__RenderProgramReady) == KP__RenderProgramReady) {
    glLinkProgram(this->name); KP__GLASSERT
    /* \todo errcheck */
    {
      GLint val;
      glGetProgramiv(this->name, GL_LINK_STATUS, &val);
      if (val != GL_TRUE) {
        glGetProgramiv(this->name, GL_INFO_LOG_LENGTH, &val);
        char *buffer = kpAlloc(val + 1);
        glGetProgramInfoLog(this->name, val + 1, NULL, buffer);
        KP__L("%p: %s", this, buffer);
        kpFree(buffer);
        return 0;
      }
    }
  }
  return 1;
}

int kpRenderProgramArgumentTag(
  KPrender_program_o program,
  const char *name,
  KPrender_tag_t tag)
{
  KP_THIS(KP__render_program_t, program);
  int location = glGetUniformLocation(this->name, name);
  if (location == -1) {
    KP__L("%p cannot find location for uniform \"%s\"", this, name);
    return 0;
  }

  for (int i = 0; i < KP__RENDER_PROGRAM_MAX_ARGS; ++i)
    if (this->args[i].tag.tag.value == 0) {
      this->args[i].tag = tag;
      this->args[i].location = location;
      KP__L("bound argument tag %c%c%c%c to location %d",
        tag.tag.name[0],tag.tag.name[1],tag.tag.name[2],tag.tag.name[3],
        location);
      return 0;
    }

  KP__L("%p cannot find free tag slot for uniform \"%s\"", this, name);
  return 0;
}

int kpRenderProgramAttributeTag(
  KPrender_program_o program,
  const char *name,
  KPrender_tag_t tag)
{
  KP_THIS(KP__render_program_t, program);
  int location = glGetAttribLocation(this->name, name);
  if (location == -1) {
    KP__L("%p cannot find location for attrib \"%s\"", this, name);
    return 0;
  }

  for (int i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i)
    if (this->attribs[i].tag.tag.value == 0) {
      this->attribs[i].tag = tag;
      this->attribs[i].location = location;
      KP__L("bound attribute tag %c%c%c%c to location %d",
        tag.tag.name[0],tag.tag.name[1],tag.tag.name[2],tag.tag.name[3],
        location);
      return 0;
    }

  KP__L("%p cannot find free tag slot for attrib \"%s\"", this, name);
  return 0;
}

/******************************************************************************/
/* batch */

#undef KP__SYS
#define KP__SYS "GL::batch"

void kp__RenderBatchDtor(void *batch) {
  KP_THIS(KP__render_batch_t, batch);
  for (int i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i) {
    if (this->attribs[i].tag.tag.value == 0)
      break;
    kpRelease(this->attribs[i].attrib.buffer);
  }

  kpRelease(this->index.buffer);
}

KPrender_batch_o kpRenderBatchCreate() {
  DECLARE_STATE;
  KP__render_batch_t *this = KP_NEW(KP__render_batch_t);
  this->O.dtor = kp__RenderBatchDtor;
  for (int i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i)
    this->attribs[i].tag.tag.value = 0;
  this->index.buffer = 0;
  return this;
}

static const GLenum KP__render_vertex_attrib_type[KPRenderVertexAttrib_Max] = {
  /*KPRenderVertexAttribF32*/ GL_FLOAT,
  /*KPRenderVertexAttribU8*/ GL_UNSIGNED_BYTE
};

int kpRenderBatchAttribSet(KPrender_batch_o batch,
  KPrender_tag_t tag,
  const KPrender_vertex_attrib_t* attrib)
{
  KP_ASSERT(attrib->type >= 0);
  KP_ASSERT(attrib->type < KPRenderVertexAttrib_Max);
  KP_THIS(KP__render_batch_t, batch);
  for (int i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i)
    if (this->attribs[i].tag.tag.value == 0) {
      this->attribs[i].tag = tag;
      this->attribs[i].attrib.buffer = kpRetain(attrib->buffer);
      this->attribs[i].attrib.components = attrib->components;
      this->attribs[i].attrib.type =
        KP__render_vertex_attrib_type[attrib->type];
      this->attribs[i].attrib.offset = attrib->offset;
      this->attribs[i].attrib.stride = attrib->stride;
      return 1;
    }
  return 0;
}

static const GLenum KP__render_draw_primitive_type[KPRenderDrawPrimitive_Max] = {
  /*KPRenderDrawPrimitivePoint*/ GL_POINTS,
  /*KPRenderDrawPrimitiveLine*/ GL_LINES,
  /*KPRenderDrawPrimitiveTriangleList*/ GL_TRIANGLES,
  /*KPRenderDrawPrimitiveTriangleStrip*/ GL_TRIANGLE_STRIP
};

static const GLenum KP__render_draw_index_type[KPRenderDrawIndex_Max] = {
  /*KPRenderDrawIndexU16*/ GL_UNSIGNED_SHORT,
  /*KPRenderDrawIndexU32*/ GL_UNSIGNED_INT
};

void kpRenderBatchDrawSet(KPrender_batch_o batch,
  const KPrender_draw_params_t* param)
{
  KP_THIS(KP__render_batch_t, batch);
  kpRetain(param->buffer);
  kpRelease(this->index.buffer);
  this->index.buffer = param->buffer;
  this->index.primitive = KP__render_draw_primitive_type[param->primitive];
  this->index.offset = param->offset;
  if (param->buffer)
    this->index.type = KP__render_draw_index_type[param->index_type];
  this->index.count = param->count;
}

/******************************************************************************/
/* GL framebuffer */

void kp__RenderFramebufferDtor(void *obj) {
  KP_THIS(KP__render_framebuffer_t, obj);
  if (this->renderbuffer_depth != 0) {
    glDeleteRenderbuffers(1, &this->renderbuffer_depth);
    KP__GLASSERT
  }
  kpRelease(this->depth);
  for (KPsize i = 0; i < this->ncolors; ++i)
    kpRelease(this->colors[i]);
  glDeleteFramebuffers(1, &this->name); KP__GLASSERT
}

void kp__RenderStateFramebufferBind(KP__render_framebuffer_o this) {
  DECLARE_STATE;
  kpRetain(this);
  if (this == 0) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); KP__GLASSERT
    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, this->name); KP__GLASSERT
  kpRelease(state->framebuffer);
  state->framebuffer = this;
}

KPrender_framebuffer_o kpRenderFramebufferCreate(
  KPrender_framebuffer_params_t* params)
{
  DECLARE_STATE;
  KP_ASSERT(params->ncolors == 1);
  KP__render_framebuffer_o this = (KP__render_framebuffer_o)kpNew(
    kp_allocator_default,
    sizeof(KP__render_framebuffer_t) +
    sizeof(KP__render_sampler_o) * params->ncolors);
  this->O.dtor = kp__RenderFramebufferDtor;

  KP__render_sampler_o *colors = (KP__render_sampler_o*)params->colors;

  this->width = colors[0]->width;
  this->height = colors[0]->height;

  glGenFramebuffers(1, &this->name); KP__GLASSERT
  glBindFramebuffer(GL_FRAMEBUFFER, this->name); KP__GLASSERT

  this->ncolors = params->ncolors;
  this->colors = (KP__render_sampler_o*)(this + 1);
  for (KPsize i = 0; i < params->ncolors; ++i) {
    this->colors[i] = kpRetain(params->colors[i]);
    KP_ASSERT(this->colors[i]->width == this->width);
    KP_ASSERT(this->colors[i]->height == this->height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i,
      GL_TEXTURE_2D, this->colors[i]->name, 0); KP__GLASSERT
  }

  this->depth = kpRetain(params->depth);
  if (this->depth == 0 && (params->flags & KPRenderFramebufferFlagDepthAny) != 0) {
    glGenRenderbuffers(1, &this->renderbuffer_depth); KP__GLASSERT
    glBindRenderbuffer(GL_RENDERBUFFER, this->renderbuffer_depth); KP__GLASSERT
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
      this->width, this->height); KP__GLASSERT
    glBindRenderbuffer(GL_RENDERBUFFER, 0); KP__GLASSERT
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_RENDERBUFFER, this->renderbuffer_depth); KP__GLASSERT
  } else if (this->depth != 0) {
    KP_ASSERT(this->depth->width == this->width);
    KP_ASSERT(this->depth->height == this->height);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
      this->depth->name); KP__GLASSERT
  }

  switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
    case GL_FRAMEBUFFER_COMPLETE: break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      KP_FAIL("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      KP_FAIL("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
      break;
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
      KP_FAIL("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
      break;
#endif
    case GL_FRAMEBUFFER_UNSUPPORTED:
      KP_FAIL("GL_FRAMEBUFFER_UNSUPPORTED");
      break;
    default:
      KP_FAIL("Unknown framebuffer status");
  }
  KP__GLASSERT

  // restore previous binding
  kp__RenderStateFramebufferBind(state->framebuffer);
  return this;
}


/*********************************** GLOBAL ***********************************/

#undef KP__SYS
#define KP__SYS "GL"

static const GLenum KP__render_depth_func[] = {
/*  KPRenderDepthFuncNever*/ GL_NEVER,
/*  KPRenderDepthFuncAlways*/ GL_ALWAYS,
/*  KPRenderDepthFuncLess*/ GL_LESS,
/*  KPRenderDepthFuncLessOrEqual*/ GL_LEQUAL,
/*  KPRenderDepthFuncGreater*/ GL_GREATER,
/*  KPRenderDepthFuncGreaterOrEqual*/ GL_GEQUAL,
/*  KPRenderDepthFuncNotEqual*/ GL_NOTEQUAL,
/*  KPRenderDepthFuncEqual*/ GL_EQUAL
};

void kpRenderSetDestination(const KPrender_destination_t *dest) {
  DECLARE_STATE;
  kp__RenderStateFramebufferBind(dest->framebuffer);

  KPu32 x = dest->viewport.bl.x, y = dest->viewport.bl.y;
  KPu32 width = dest->viewport.tr.x - dest->viewport.bl.x;
  KPu32 height = dest->viewport.tr.y - dest->viewport.bl.y;

  KP__render_framebuffer_o fb = (KP__render_framebuffer_o)dest->framebuffer;

  if (width == 0 || height == 0) {
    KP_ASSERT(fb != 0);
    x = y = 0;
    width = fb->width;
    height = fb->height;
  }
  /*KP__L("%d %d %d %d", x, y, width, height);*/
  glViewport(x, y, width, height); KP__GLASSERT

  if (dest->depth.test == KPRenderDepthTestEnabled)
  {
    glEnable(GL_DEPTH_TEST); KP__GLASSERT
    glDepthMask(dest->depth.write == KPRenderDepthWriteEnabled); KP__GLASSERT
    KP_ASSERT(dest->depth.func >= KPRenderDepthFuncNever);
    KP_ASSERT(dest->depth.func <= KPRenderDepthFuncEqual);
    glDepthFunc(KP__render_depth_func[dest->depth.func]); KP__GLASSERT
  } else {
    glDisable(GL_DEPTH_TEST); KP__GLASSERT
  }
}

static void kp__RenderCommandFill(KPrender_cmd_fill_t *cmd) {
  glClearColor(cmd->color.x, cmd->color.y, cmd->color.z, cmd->color.w); KP__GLASSERT
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); KP__GLASSERT
}

static void kp__RenderCommandRasterize(KPrender_cmd_rasterize_t *cmd) {
  DECLARE_STATE;
  KP__render_program_t *program = (KP__render_program_t*)cmd->program;
  KP__render_batch_t *batch = (KP__render_batch_t*)cmd->batch;

  /* setup program */
  if (state->program != program) {
    state->program = program;
    glUseProgram(program->name);
  }

  /* setup program arguments */
  kp__RenderStateSamplerGroupBegin(state);
  int i;
  for (i = 0; i < KP__RENDER_PROGRAM_MAX_ARGS; ++i) {
      /*KP__L("i%d: %c%c%c%c", i,
        program->args[i].tag.tag.name[0],
        program->args[i].tag.tag.name[1],
        program->args[i].tag.tag.name[2],
        program->args[i].tag.tag.name[3]);*/
    if (program->args[i].tag.tag.value == 0)
      break;
    KPu32 j = 0;
    for (; j < cmd->env_count; ++j) {
      KP__render_program_env_t *env = (KP__render_program_env_t*)cmd->env[j];
      int k = 0;
      for (; k < KP__RENDER_PROGRAM_ENV_MAX_VALUES; ++k) {
        /*KP__L("i%d j%d k%d: %p %c%c%c%c", i, j, k, env,
          env->values[k].tag.tag.name[0],
          env->values[k].tag.tag.name[1],
          env->values[k].tag.tag.name[2],
          env->values[k].tag.tag.name[3]);*/
        if (env->values[k].tag.tag.value == program->args[i].tag.tag.value) {
          kp__RenderProgramEnvApply(state, env, k, program->args[i].location);

          /* skip all further environments */
          j = cmd->env_count;
          break;
        }
      }
    } /* for all envs */
  } /* for all arguments */

  /* setup batch */
  for (i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i) {
    if (program->attribs[i].tag.tag.value == 0)
      break;
    int j;
    for (j = 0; j < KP__RENDER_BATCH_MAX_ATTRIBS; ++j) {
      if (program->attribs[i].tag.tag.value == batch->attribs[j].tag.tag.value) {
        KPrender_vertex_attrib_t *a = &batch->attribs[j].attrib;
        kp__RenderStateBufferBind(state, a->buffer, KP__RenderBufferTargetArray);
        /*KP__L("glVertexAttribPointer(%d, %d, %d, %d, %p)",
          program->attribs[i].location,
          a->components, a->type, a->stride, (void*)a->offset);*/
        glVertexAttribPointer(program->attribs[i].location,
          a->components, a->type, GL_TRUE, a->stride, (void*)a->offset); KP__GLASSERT
        glEnableVertexAttribArray(program->attribs[i].location); KP__GLASSERT
        break;
      } /* for matching batch attrib */
    } /* for all batch attribs */
  } /* for all program attribs */

  /* draw */
  if (batch->index.buffer != 0) {
    kp__RenderStateBufferBind(state, batch->index.buffer, KP__RenderBufferTargetElementArray);
    /*KP__L("glDrawElements(%d, %d, %d, %p)", batch->index.primitive,
      batch->index.count, batch->index.type, (void*)batch->index.offset);*/
    glDrawElements(batch->index.primitive, batch->index.count,
      batch->index.type, (void*)batch->index.offset); KP__GLASSERT
  } else {
    kp__RenderStateBufferBind(state, 0, KP__RenderBufferTargetElementArray);
    /*KP__L("glDrawArrays(%d, %d, %d)", batch->index.primitive,
      batch->index.offset, batch->index.count);*/
    KP_ASSERT(batch->index.offset < INT_MAX);
    glDrawArrays(batch->index.primitive,
      (int)batch->index.offset, batch->index.count); KP__GLASSERT
  }

  /* cleanup */
  for (i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i)
    if (program->attribs[i].tag.tag.value != 0) {
      glDisableVertexAttribArray(program->attribs[i].location); KP__GLASSERT
    }
  kp__RenderStateSamplerGroupEnd(state);
}

void kpRenderExecuteCommand(const KPrender_cmd_header_t *command) {
  switch (command->cmd) {
  case KPrender_Command_Fill:
    kp__RenderCommandFill((KPrender_cmd_fill_t*)command);
    break;
  case KPrender_Command_Rasterize:
    kp__RenderCommandRasterize((KPrender_cmd_rasterize_t*)command);
    break;

  default:
    KP_ASSERT(!"Invalid command");
  }
}
