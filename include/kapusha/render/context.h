#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*KPglcontext_dtor_f)(void *);

typedef struct {
  void *param;
  void (*dtor)(void *param);
} KPglcontext_i;

static inline kpGLContextDtor(KPglcontext_i *ctx) {
  ctx->dtor(ctx->param);
}

void kpGLContextMakeCurrent(KPglcontext_i *ctx);

#ifdef __cplusplus
} // extern "C"
#endif
