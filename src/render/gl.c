#include "../render/gl.h"

#define KP__SYS "GL"

void kp__GlAssert(const char *file, int line) {
  int err = glGetError();
  if (err == 0) return;
  const char *errname;
  switch (err) {
    case GL_INVALID_VALUE: errname = "GL_INVALID_VALUE"; break;
    case GL_INVALID_OPERATION: errname = "GL_INVALID_OPERATION"; break;
    default: errname = "UNKNOWN";
  }
  KP__L("error %s[%d] @ %s:%d", errname, err, file, line);
  kpSysExit(-1);
}

inline static KP__render_state_t *kp__RenderState() {
  /* \todo multithreading */
  static KP__render_state_t state;
  return &state;
}

/******************************************************************************/
/* buffer */

#undef KP__SYS
#define KP__SYS "GL::buffer"

static GLenum KP__render_buffer_target_gl[KP__RenderBufferTarget_MAX] = {
  GL_ARRAY_BUFFER,
  GL_ELEMENT_ARRAY_BUFFER
};

#define DECLARE_STATE \
  KP__render_state_t *state = kp__RenderState(); KP_ASSERT(state)

static void kp__RenderBufferBind(
    KP__render_buffer_t *this, KP__RenderBufferTarget target) {
  KP_ASSERT(this->name != 0);
  KP_ASSERT(target < KP__RenderBufferTarget_MAX);
  KP__render_state_t *state = kp__RenderState(); KP_ASSERT(state);
  if (state->buffer_targets_[target] != this) {
    glBindBuffer(KP__render_buffer_target_gl[target], this->name); KP__GLASSERT
  }
  state->buffer_targets_[target] = this;
}

static void kp__RenderBufferUnbind(KP__render_buffer_t *this) {
  KP__render_state_t *state = kp__RenderState(); KP_ASSERT(state);
  int i;
  for (i = 0; i < KP__RenderBufferTarget_MAX; ++i)
    if (this == state->buffer_targets_[i]) {
      glBindBuffer(KP__render_buffer_target_gl[i], 0); KP__GLASSERT
      state->buffer_targets_[i] = 0;
    }
}

static void kp__RenderBufferDtor(void *buffer) {
  KP_THIS(KP__render_buffer_t, buffer);
  kp__RenderBufferUnbind(this);
  KP__L("%p: delete %d", this, this->name);
  glDeleteBuffers(1, &this->name); KP__GLASSERT
}

KPrender_buffer_o kpRenderBufferCreate() {
  DECLARE_STATE;
  KP__render_buffer_t *this = KP_NEW(KP__render_buffer_t, state->heap);
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
  KP_THIS(KP__render_buffer_t, buffer);
  KP__RenderBufferTarget target = (flags & KPRenderBufferFlagElement) ?
    KP__RenderBufferTargetElementArray : KP__RenderBufferTargetArray;
  kp__RenderBufferBind(this, target);
  KP__L("%p: upload %d", this, (int)data.size);
  glBufferData(KP__render_buffer_target_gl[target], data.size, data.data,
    (flags & KPRenderBufferFlagDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  KP__GLASSERT

}

/******************************************************************************/
/* program environment */

#undef KP__SYS
#define KP__SYS "GL::env"

KPrender_program_env_o kpRenderProgramEnvCreate() {
  DECLARE_STATE;
  KP__render_program_env_t *this = KP_NEW(KP__render_program_env_t, state->heap);
  int i;
  for (i = 0; i < KP__RENDER_PROGRAM_ENV_MAX_VALUES; ++i)
    this->values[i].tag.tag.value = 0;
  return this;
}

int kp__RenderProgramEnvSetNScalar(
  KP__render_program_env_t *this,
  KPrender_tag_t tag,
  KP__RenderProgramEnvValueType type,
  KPscalarf *value,
  int n)
{
  KP_ASSERT(n > 0);
  KP_ASSERT(n <= 16);
  int i;
  for (i = 0; i < KP__RENDER_PROGRAM_ENV_MAX_VALUES; ++i)
    if (this->values[i].tag.tag.value == 0
        || this->values[i].tag.tag.value == tag.tag.value) {
      this->values[i].tag = tag;
      this->values[i].type = type;
      kpMemcpy(&this->values[i].v.f, value, sizeof(KPscalarf) * n);
      return 1;
    }
  return 0;
}

int kpRenderProgramEnvSetScalarf(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  KPscalarf value)
{
  KP_THIS(KP__render_program_env_t, env);
  KP__L("%p set %c%c%c%c = %f", this,
    tag.tag.name[0],tag.tag.name[1],tag.tag.name[2],tag.tag.name[3], value);
  return kp__RenderProgramEnvSetNScalar(this, tag,
    KP__RenderProgramEnvValueScalarf, &value, 1);
}

int kpRenderProgramEnvSetVec4f(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  KPvec4f *value)
{
  KP_THIS(KP__render_program_env_t, env);
  return kp__RenderProgramEnvSetNScalar(this, tag,
    KP__RenderProgramEnvValueVec4f, &value->x, 4);
}

int kpRenderProgramEnvSetMat4f(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  KPmat4f *value)
{
  KP_THIS(KP__render_program_env_t, env);
  return kp__RenderProgramEnvSetNScalar(this, tag,
    KP__RenderProgramEnvValueMat4f, &value->rows[0].x, 16);
}

static void kp__RenderProgramEnvApply(
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
    default:
      KP_FAIL(!"Impossible KP__RenderProgramEnvValueType %d", value->type);
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
  KP__render_program_t *this = KP_NEW(KP__render_program_t, state->heap);
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
  KP_ASSERT(data.size < INT_MAX);
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
  glAttachShader(this->name, shader); KP__GLASSERT
  /* \todo errcheck */
  this->flags |= flag;
  if ((this->flags & KP__RenderProgramReady) == KP__RenderProgramReady) {
    glLinkProgram(this->name); KP__GLASSERT
    /* \todo errcheck */
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
    /* \todo KP_LOG_ERR(); */
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

  /* \todo KP_LOG_ERR(); */
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
    /* \todo KP_LOG_ERR(); */
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

  /* \todo KP_LOG_ERR(); */
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
  KP__render_batch_t *this = KP_NEW(KP__render_batch_t, state->heap);
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
  kpRelease(this->index.buffer);
  this->index.buffer = kpRetain(param->buffer);
  this->index.primitive = KP__render_draw_primitive_type[param->primitive];
  this->index.offset = param->offset;
  if (param->buffer)
    this->index.type = KP__render_draw_index_type[param->index_type];
  this->index.count = param->count;
}

/*********************************** GLOBAL ***********************************/

#undef KP__SYS
#define KP__SYS "GL"

void kpRenderSetDestination(const KPrender_destination_t *dest) {
  KP__L("glViewport(%d, %d, %d, %d)",
    dest->viewport.bl.x, dest->viewport.bl.y,
    dest->viewport.tr.x - dest->viewport.bl.x,
    dest->viewport.tr.y - dest->viewport.bl.y);
  glViewport(
    dest->viewport.bl.x, dest->viewport.bl.y,
    dest->viewport.tr.x - dest->viewport.bl.x,
    dest->viewport.tr.y - dest->viewport.bl.y);
}

static void kp__RenderCommandFill(KPrender_cmd_fill_t *cmd) {
  glClearColor(cmd->color.x, cmd->color.y, cmd->color.z, cmd->color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void kp__RenderCommandRasterize(KPrender_cmd_rasterize_t *cmd) {
  DECLARE_STATE;
  KP__render_program_t *program = (KP__render_program_t*)cmd->program;
  KP__render_batch_t *batch = (KP__render_batch_t*)cmd->batch;

  /* setup program */
  if (state->program_ != program) {
    state->program_ = program;
    glUseProgram(program->name);
  }

  /* setup program arguments */
  int i;
  for (i = 0; i < KP__RENDER_PROGRAM_MAX_ARGS; ++i) {
    if (program->args[i].location == -1)
      break;
    KPu32 j = 0;
    for (; j < cmd->env_count; ++j) {
      int k = 0;
      for (; k < KP__RENDER_PROGRAM_ENV_MAX_VALUES; ++k) {
        KP__render_program_env_t *env = (KP__render_program_env_t*)&cmd->env[j];
        if (env->values[k].tag.tag.value == program->args[i].tag.tag.value) {
          kp__RenderProgramEnvApply(env, k, program->args[i].location);

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
        kp__RenderBufferBind(a->buffer, KP__RenderBufferTargetArray);
        KP__L("glVertexAttribPointer(%d, %d, %d, %d, %p)",
          program->attribs[i].location,
          a->components, a->type, a->stride, (void*)a->offset);
        glVertexAttribPointer(program->attribs[i].location,
          a->components, a->type, GL_FALSE, a->stride, (void*)a->offset); KP__GLASSERT
        glEnableVertexAttribArray(program->attribs[i].location); KP__GLASSERT
        break;
      } /* for matching batch attrib */
    } /* for all batch attribs */
  } /* for all program attribs */

  /* draw */
  if (batch->index.buffer != 0) {
    kp__RenderBufferBind(batch->index.buffer, KP__RenderBufferTargetElementArray);
      glDrawElements(batch->index.primitive, batch->index.count,
        batch->index.type, (void*)batch->index.offset); KP__GLASSERT
  } else {
    KP__L("glDrawArrays(%d, %d, %d)", batch->index.primitive,
      batch->index.offset, batch->index.count);
    glDrawArrays(batch->index.primitive,
      batch->index.offset, batch->index.count); KP__GLASSERT
  }

  /* cleanup */
  for (i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i)
    if (program->attribs[i].tag.tag.value != 0) {
      glDisableVertexAttribArray(program->attribs[i].location); KP__GLASSERT
    }
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
