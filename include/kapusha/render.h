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
  KPrender_tag_t tag;
  tag.tag.name[0] = name[0] ? *(name++) : 0;
  tag.tag.name[1] = name[0] ? *(name++) : 0;
  tag.tag.name[2] = name[0] ? *(name++) : 0;
  tag.tag.name[3] = name[0] ? *(name++) : 0;
  return tag;
}

typedef enum {
  KPRenderProgramValueScalarf,
  KPRenderProgramValueVec2f,
  KPRenderProgramValueVec3f,
  KPRenderProgramValueVec4f,
  KPRenderProgramValueMat2f,
  KPRenderProgramValueMat3f,
  KPRenderProgramValueMat4f,
  KPRenderProgramValueSampler
} KPRenderProgramValueType;

typedef struct {
  KPRenderProgramValueType type;
  union {
    KPrender_sampler_o sampler;
    KPscalarf f[16];
  } value;
} KPrender_program_value_t;

/******************************************************************************/
/* buffer */

typedef void* KPrender_buffer_o;
typedef enum {
  KPRenderBufferFlagNone = 0x0000,
  KPRenderBufferFlagElement = 0x0001,
  KPRenderBufferFlagDynamic = 0x0010
} KPRenderBufferFlags;

KPrender_buffer_o kpRenderBufferCreate();
void kpRenderBuffer(KPrender_buffer_o buffer);
void kpRenderBufferUpload(KPrender_buffer_o buffer,
  KPblob_desc_t data, KPu32 flags);

/******************************************************************************/
/* program */

typedef void* KPrender_program_o;
typedef enum {
  KPRenderProgramModuleVertex,
  KPRenderProgramModuleFragment,
} KPRenderProgramModuleType;

KPrender_program_o kpRenderProgramCreate();
int kpRenderProgramModuleSet(KPrender_program_o program,
  KPRenderProgramModuleType module, KPblob_desc_t data);
void kpRenderProgramArgumentSet(KPrender_program_o program,
  KPrender_tag_t tag, const char *name,
  KPrender_program_value_t value);
void kpRenderProgramAttributeSet(KPrender_program_o program,
  KPrender_tag_t tag, const char *name);
  
/******************************************************************************/
/* sampler */

/* \todo typedef enum {
  KPRenderSampler1D,
  KPRenderSampler2D,
  KPRenderSampler3D
} KPRenderSamplerType;
KPrender_sampler_o kpRenderSamplerCreate();
void kpRenderSamplerLoad();
*/

/******************************************************************************/
/* program environment */

typedef struct {
  struct {
    KPrender_tag_t tag;
    KPrender_program_value_t value;
  } value[KP_MAX_PROGRAM_ENV_VALUES];
} KPrender_program_env_t;

void kpRenderProgramEnvClear(KPrender_program_env_t *env);
void kpRenderProgramEnvSet(KPrender_program_env_t *env, KPrender_tag_t tag,
  KPrender_program_value_t *value);

/******************************************************************************/
/* raster batch */

typedef void* KPrender_batch_o;

typedef enum {
  KPRenderVertexAttribF32,
  KPRenderVertexAttribU8
} KPRenderVertexAttribType;

typedef struct {
  KPrender_buffer_o buffer;
  KPu32 components;
  KPRenderVertexAttribType type;
  KPu32 offset;
  KPu32 stride;
} KPrender_vertex_attrib_t;

typedef enum {
  KPRenderDrawPrimitivePoint,
  KPRenderDrawPrimitiveLine,
  KPRenderDrawPrimitiveTriangleList,
  KPRenderDrawPrimitiveTriangleStrip
} KPRenderDrawPrimitiveType;

typedef struct {
  KPRenderDrawPrimitiveType primitive;
  KPrender_buffer_o buffer;
  void *data;
  KPu32 offset;
  KPu32 count;
} KPrender_draw_params_t;

KPrender_batch_o kpRenderBatchCreate();
void kpRenderBatchAttribSet(KPrender_batch_o,
  KPrender_tag_t tag,
  const KPrender_vertex_attrib_t*);
void kpRenderBatchDrawSet(KPrender_batch_o,
  const KPrender_draw_params_t*);
  
/******************************************************************************/
/* destination */

typedef struct {
/*! \todo
  KPrender_framebuffer_o *framebuffer;
  int depth_mode;
  int stencil_mode;
  int color_mode;
  */
  KPrect2i viewport;
} KPrender_destination_t;

static void kpRenderDestinationDefaults(KPrender_destination_t *dest) {
  dest->viewport.bl.x = dest->viewport.bl.y =
  dest->viewport.tr.x = dest->viewport.tr.y = 0;
}

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
  /*! \todo KPscalarf depth; */
  /*! \todo KPu32 stencil; */
} KPrender_cmd_fill_t;

typedef struct {
  KPrender_cmd_header_t header;
  KPrender_batch_o batch;
  KPrender_program_o program;
  KPrender_program_env_t *env;
  KPu32 env_count;
} KPrender_cmd_rasterize_t;

void kpRenderExecuteCommand(const KPrender_cmd_header_t *command);

#ifdef __cplusplus
} // extern "C"
#endif
