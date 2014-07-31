#include "../render/gl.h"

/******************************************************************************/
/* buffer */

static GLenum KP__render_buffer_target_gl[KP__RenderBufferTarget_MAX] = {
  GL_ARRAY_BUFFER,
  GL_ELEMENT_ARRAY_BUFFER
};

inline static KP__render_state_t *kp__RenderState() {
  /* \todo multithreading */
  static KP__render_state_t state;
  return &state;
}

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
  for (int i = 0; i < KP__RenderBufferTarget_MAX; ++i)
    if (this == state->buffer_targets_[i]) {
      glBindBuffer(KP__render_buffer_target_gl[i], 0); KP__GLASSERT
      state->buffer_targets_[i] = NULL;
    }
}

static void kp__RenderBufferDtor(void *buffer) {
  KP_THIS(KP__render_buffer_t, buffer);
  kp__RenderBufferUnbind(this);
  glDeleteBuffers(1, &this->name); KP__GLASSERT
}

KPrender_buffer_o kpRenderBufferCreate() {
  DECLARE_STATE;
  KP__render_buffer_t *this = KP_NEW(KP__render_buffer_t, state->heap);
  this->O.dtor = kp__RenderBufferDtor;
  glGenBuffers(1, &this->name); KP__GLASSERT
  KP_ASSERT(this->name != 0);
  return this;
}

void kpRenderBufferUpload(KPrender_buffer_o buffer,
    KPblob_desc_t data, KPu32 flags) {
  KP_THIS(KP__render_buffer_t, buffer);
  KP__RenderBufferTarget target = (flags & KPRenderBufferFlagElement) ?
    KP__RenderBufferTargetElementArray : KP__RenderBufferTargetArray;
  kp__RenderBufferBind(this, target);
  glBufferData(KP__render_buffer_target_gl[target], data.size, data.data,
    (flags & KPRenderBufferFlagDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

/******************************************************************************/
/* program */

static void kp__RenderProgramDtor(void *buffer) {
  KP_THIS(KP__render_program_t, buffer);
  glDeleteProgram(this->name); KP__GLASSERT
}

KPrender_program_o kpRenderProgramCreate() {
  DECLARE_STATE;
  KP__render_program_t *this = KP_NEW(KP__render_program_t, state->heap);
  this->O.dtor = kp__RenderProgramDtor;
  this->name = glCreateProgram(); KP__GLASSERT
  for (int i = 0; i < KP__RenderProgramShader_MAX; ++i) this->shaders[i] = 0;
  return this;
}

int kpRenderProgramModuleSet(KPrender_program_o program,
    KPRenderProgramModuleType module, KPblob_desc_t data) {
  KP_THIS(KP__render_program_t, program);
  KP_ASSERT(data.size < INT_MAX);
  KPu32 flag;
  GLint shader;
  switch (module) {
  case KPRenderProgramModuleVertex:
    flag = KP__RenderProgramHaveVertex;
    shader = this->shaders[KP__RenderProgramShaderVertex];
    if (shader == 0)
      this->shaders[KP__RenderProgramShaderVertex] = shader
        = glCreateShader(GL_VERTEX_SHADER);
    break;
  case KPRenderProgramModuleFragment:
    flag = KP__RenderProgramHaveFragment;
    shader = this->shaders[KP__RenderProgramShaderFragment];
    if (shader == 0)
      this->shaders[KP__RenderProgramShaderFragment] = shader
        = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  }
  GLint length = (GLint)data.size;
  glShaderSource(shader, 1, (const GLchar**)&data.data, &length);
  glCompileShader(shader);
  glAttachShader(this->name, shader);
  /* \todo errcheck */
  this->flags |= flag;
  if ((this->flags & KP__RenderProgramReady) == KP__RenderProgramReady) {
    glLinkProgram(this->name);
    /* \todo errcheck */
  }
  return 1;
}

static void kp__RenderProgramArgument(KP__render_program_t *program,
    int location, const KPrender_program_value_t *value) {
  switch (value->type) {
    case KPRenderProgramValueScalarf:
      glUniform1f(location, value->value.f[0]); KP__GLASSERT
      break;
    case KPRenderProgramValueVec2f:
      glUniform2f(location, value->value.f[0], value->value.f[1]); KP__GLASSERT
      break;
    case KPRenderProgramValueVec3f:
      glUniform3f(location,
        value->value.f[0],
        value->value.f[1],
        value->value.f[2]); KP__GLASSERT
      break;
    case KPRenderProgramValueVec4f:
      glUniform4f(location,
        value->value.f[0],
        value->value.f[1],
        value->value.f[2],
        value->value.f[3]); KP__GLASSERT
      break;
    case KPRenderProgramValueMat2f:
      glUniformMatrix2fv(location, 1, GL_FALSE, value->value.f); KP__GLASSERT
      break;
    case KPRenderProgramValueMat3f:
      glUniformMatrix3fv(location, 1, GL_FALSE, value->value.f); KP__GLASSERT
      break;
    case KPRenderProgramValueMat4f:
      glUniformMatrix4fv(location, 1, GL_FALSE, value->value.f); KP__GLASSERT
      break;
    case KPRenderProgramValueSampler:
      KP_ASSERT(!"Not implemented");
    default:
      KP_ASSERT(!"Impossible");
  }
}

/*********************************** GLOBAL ***********************************/

void kpRenderSetDestination(const KPrender_destination_t *dest) {
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
  for (int i = 0; i < KP__RENDER_PROGRAM_MAX_ARGS; ++i) {
    if (program->args[i].location == -1)
      break;
    int j = 0;
    for (; j < cmd->env_count; ++j) {
      int k = 0;
      for (; k < KP_MAX_PROGRAM_ENV_VALUES; ++k) {
        if (cmd->env->value[k].tag.tag.value == program->args[i].tag.tag.value) {
          kp__RenderProgramArgument(program,
            program->args[i].location, &cmd->env->value[k].value);
          j = cmd->env_count;
          break;
        }
      }
    } /* for all envs */

    /* apply default value */
    if (j != cmd->env_count) {
      kp__RenderProgramArgument(program,
        program->args[i].location, &program->args[i].value);
    }
  } /* for all arguments */

  /* setup batch */
  for (int i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i) {
    if (program->attribs[i].tag.tag.value == 0)
      break;
    for (int j = 0; j < KP__RENDER_BATCH_MAX_ATTRIBS; ++j) {
      if (program->attribs[i].tag.tag.value == batch->attribs[j].tag.tag.value) {
        KPrender_vertex_attrib_t *a = &batch->attribs[j].attrib;
        kp__RenderBufferBind(a->buffer, KP__RenderBufferTargetArray);
        glVertexAttribPointer(program->attribs[i].location,
          a->components, a->type, GL_FALSE, a->stride, a->offset); KP__GLASSERT
        glEnableVertexAttribArray(program->attribs[i].location); KP__GLASSERT
        break;
      } /* for matching batch attrib */
    } /* for all batch attribs */
  } /* for all program attribs */
  
  /* draw */
  if (batch->index.buffer != NULL) {
    kp__RenderBufferBind(batch->index.buffer, KP__RenderBufferTargetElementArray);
      glDrawElements(batch->index.primitive, batch->index.count,
        batch->index.type, batch->index.offset); KP__GLASSERT
  } else {
    glDrawArrays(batch->index.primitive,
      batch->index.offset, batch->index.count); KP__GLASSERT
  }
  
  /* cleanup */
  for (int i = 0; i < KP__RENDER_BATCH_MAX_ATTRIBS; ++i)
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
