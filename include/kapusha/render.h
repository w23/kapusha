#pragma once

#include "kapusha/sys.h"
#include "kapusha/math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
} KPrender_program_t;

typedef struct {
} KPrender_program_env_t;

typedef struct {
} KPrender_buffer_t;

typedef struct {
} KPrender_sampler_t;

typedef struct {
} KPrender_framebuffer_t;

typedef struct KPrender_mesh_t {
} KPrender_mesh_t;

typedef struct {
/*! \todo
  KPrender_framebuffer_t *framebuffer;
  int depth_mode;
  int stencil_mode;
  int color_mode;
  */
  KPrect2i viewport;
} KPrender_destination_t;

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
  KPrender_mesh_t *mesh;
  KPrender_program_t *program;
  KPrender_program_env_t *env;
} KPrender_cmd_raster_t;

static void kpRenderDestinationDefaults(KPrender_destination_t *dest) {
  dest->viewport.bl.x = dest->viewport.bl.y =
  dest->viewport.tr.x = dest->viewport.tr.y = 0;
}

void kpRenderSetDestination(const KPrender_destination_t *dest);
void kpRenderExecuteCommand(const KPrender_cmd_header_t *command);

#ifdef __cplusplus
} // extern "C"
#endif
