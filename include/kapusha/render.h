#pragma once

#include "kapusha/core.h"
#include "kapusha/math.h"

#define KP_MAX_PROGRAM_ENV_VALUES 16

#ifdef __cplusplus
extern "C" {
#endif

typedef void* KPrender_sampler_o;

typedef struct {
  union {
    char name[4];
    KPu32 value;
  } tag;
} KPrender_tag_t;

inline static KPrender_tag_t kpRenderTag(const char *name) {
  KP_ASSERT(name);
  KP_ASSERT(name[0] != 0);
  KP_ASSERT(name[1] != 0);
  KP_ASSERT(name[2] != 0);
  KP_ASSERT(name[3] != 0);
  KPrender_tag_t tag;
  tag.tag.name[0] = name[0];
  tag.tag.name[1] = name[1];
  tag.tag.name[2] = name[2];
  tag.tag.name[3] = name[3];
  return tag;
}

/******************************************************************************/
/* buffer */

typedef void* KPrender_buffer_o;
typedef enum {
  KPRenderBufferFlagNone = 0x0000,
  KPRenderBufferFlagElement = 0x0001,
  KPRenderBufferFlagDynamic = 0x0010
} KPRenderBufferFlags;

KPrender_buffer_o kpRenderBufferCreate();

void kpRenderBufferUpload(
  KPrender_buffer_o buffer,
  KPu32 flags,
  KPblob_desc_t data);

/******************************************************************************/
/* program */

typedef void* KPrender_program_o;
typedef enum {
  KPRenderProgramModuleVertex,
  KPRenderProgramModuleFragment,
} KPRenderProgramModuleType;

KPrender_program_o kpRenderProgramCreate();

int kpRenderProgramModuleSet(
  KPrender_program_o program,
  KPRenderProgramModuleType module,
  KPblob_desc_t data);

int kpRenderProgramArgumentTag(
  KPrender_program_o program,
  const char *name,
  KPrender_tag_t tag);

int kpRenderProgramAttributeTag(
  KPrender_program_o program,
  const char *name,
  KPrender_tag_t tag);

/******************************************************************************/
/* sampler */

KPrender_sampler_o kpRenderSamplerCreate();

int kpRenderSamplerAlloc(KPrender_sampler_o s,
  KPu32 width, KPu32 height, KPSurfaceFormat format);
int kpRenderSamplerUpload(KPrender_sampler_o s, KPsurface_o);

/******************************************************************************/
/* program environment */

typedef void* KPrender_program_env_o;

KPrender_program_env_o kpRenderProgramEnvCreate();

int kpRenderProgramEnvSetScalarf(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  KPf32 value);

int kpRenderProgramEnvSetVec4f(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  const KPvec4f *value);

int kpRenderProgramEnvSetMat4f(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  const KPmat4f *value);

int kpRenderProgramEnvSetSampler(
  KPrender_program_env_o env,
  KPrender_tag_t tag,
  KPrender_sampler_o sampler);

/******************************************************************************/
/* raster batch */

typedef void* KPrender_batch_o;

typedef enum {
  KPRenderVertexAttribF32,
  KPRenderVertexAttribU8,
  KPRenderVertexAttrib_Max
} KPRenderVertexAttribType;

typedef struct {
  KPrender_buffer_o buffer;
  KPu32 components;
  KPRenderVertexAttribType type;
  KPuptr offset;
  KPu32 stride;
} KPrender_vertex_attrib_t;

typedef enum {
  KPRenderDrawPrimitivePoint,
  KPRenderDrawPrimitiveLine,
  KPRenderDrawPrimitiveTriangleList,
  KPRenderDrawPrimitiveTriangleStrip,
  KPRenderDrawPrimitive_Max
} KPRenderDrawPrimitiveType;

typedef enum {
  KPRenderDrawIndexU16,
  KPRenderDrawIndexU32,
  KPRenderDrawIndex_Max,
} KPRenderDrawIndexType;

typedef struct {
  KPRenderDrawPrimitiveType primitive;
  KPRenderDrawIndexType index_type;
  KPrender_buffer_o buffer;
  KPuptr offset;
  KPu32 count;
} KPrender_draw_params_t;

KPrender_batch_o kpRenderBatchCreate();
int kpRenderBatchAttribSet(KPrender_batch_o,
  KPrender_tag_t tag,
  const KPrender_vertex_attrib_t*);
void kpRenderBatchDrawSet(KPrender_batch_o,
  const KPrender_draw_params_t*);

/******************************************************************************/
/* framebuffer */

typedef void *KPrender_framebuffer_o;

enum KPrender_framebuffer_flags_t {
  KPRenderFramebufferFlagDepthAny = 0x01,
  KPRenderFramebufferFlagDepthMax = 0x07,
};

typedef struct KPrender_framebuffer_params_t {
  KPsize ncolors;
  KPrender_sampler_o *colors;
  KPrender_sampler_o depth;
  KPu32 flags;
} KPrender_framebuffer_params_t;

KPrender_framebuffer_o kpRenderFramebufferCreate(KPrender_framebuffer_params_t*);

/******************************************************************************/
/* destination */

typedef struct {
  KPrect2i viewport;
  KPrender_framebuffer_o framebuffer;

  struct {
    enum {
      KPRenderDepthTestDisabled,
      KPRenderDepthTestEnabled
    } test;
    enum {
      KPRenderDepthWriteDisabled,
      KPRenderDepthWriteEnabled
    } write;
    enum {
      KPRenderDepthFuncNever,
      KPRenderDepthFuncAlways,
      KPRenderDepthFuncLess,
      KPRenderDepthFuncLessOrEqual,
      KPRenderDepthFuncGreater,
      KPRenderDepthFuncGreaterOrEqual,
      KPRenderDepthFuncNotEqual,
      KPRenderDepthFuncEqual
    } func;
  } depth;
} KPrender_destination_t;

void kpRenderSetDestination(const KPrender_destination_t *dest);

/******************************************************************************/
/* commands */

typedef enum {
  KPrender_Command_Fill,
  KPrender_Command_Rasterize
} KPrender_Command;

typedef struct {
  KPrender_Command cmd;
  /*! \todo KPu32 flags; */
} KPrender_cmd_header_t;

typedef struct {
  KPrender_cmd_header_t header;
  KPvec4f color;
  /*! \todo flags */
  /*! \todo KPf32 depth; */
  /*! \todo KPu32 stencil; */
} KPrender_cmd_fill_t;

typedef struct {
  KPrender_cmd_header_t header;
  KPrender_batch_o batch;
  KPrender_program_o program;
  KPrender_program_env_o *env;
  KPu32 env_count;
} KPrender_cmd_rasterize_t;

void kpRenderExecuteCommand(const KPrender_cmd_header_t *command);

#ifdef __cplusplus
} // extern "C"
#endif
