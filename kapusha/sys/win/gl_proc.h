#pragma once

//! \todo auto-generate this
#define KP_GL_PROCS \
  KP_GL_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers); \
  KP_GL_PROC(PFNGLGENBUFFERSPROC, glGenBuffers); \
  KP_GL_PROC(PFNGLBLENDCOLORPROC, glBlendColor); \
  KP_GL_PROC(PFNGLBLENDEQUATIONPROC, glBlendEquation); \
  KP_GL_PROC(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate); \
  KP_GL_PROC(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate); \
  KP_GL_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv); \
  KP_GL_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog); \
  KP_GL_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv); \
  KP_GL_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog); \
  KP_GL_PROC(PFNGLDELETESHADERPROC, glDeleteShader); \
  KP_GL_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram); \
  KP_GL_PROC(PFNGLCREATESHADERPROC, glCreateShader); \
  KP_GL_PROC(PFNGLATTACHSHADERPROC,  glAttachShader); \
  KP_GL_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram); \
  KP_GL_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram); \
  KP_GL_PROC(PFNGLSHADERSOURCEPROC, glShaderSource); \
  KP_GL_PROC(PFNGLCOMPILESHADERPROC, glCompileShader); \
  KP_GL_PROC(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation); \
  KP_GL_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation); \
  KP_GL_PROC(PFNGLUNIFORM1FVPROC, glUniform1fv); \
  KP_GL_PROC(PFNGLUNIFORM2FVPROC, glUniform2fv); \
  KP_GL_PROC(PFNGLUNIFORM3FVPROC, glUniform3fv); \
  KP_GL_PROC(PFNGLUNIFORM4FVPROC, glUniform4fv); \
  KP_GL_PROC(PFNGLUNIFORMMATRIX2FVPROC, glUniformMatrix2fv); \
  KP_GL_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv); \
  KP_GL_PROC(PFNGLUNIFORM1IPROC, glUniform1i); \
  KP_GL_PROC(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers); \
  KP_GL_PROC(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers); \
  KP_GL_PROC(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers); \
  KP_GL_PROC(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D); \
  KP_GL_PROC(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus); \
  KP_GL_PROC(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers); \
  KP_GL_PROC(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer); \
  KP_GL_PROC(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage); \
  KP_GL_PROC(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer); \
  KP_GL_PROC(PFNGLBINDBUFFERPROC, glBindBuffer); \
  KP_GL_PROC(PFNGLUSEPROGRAMPROC, glUseProgram); \
  KP_GL_PROC(PFNGLACTIVETEXTUREPROC, glActiveTexture); \
  KP_GL_PROC(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer); \
  KP_GL_PROC(PFNGLDRAWBUFFERSPROC, glDrawBuffers); \
  KP_GL_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData); \
  KP_GL_PROC(PFNGLBUFFERDATAPROC, glBufferData); \
  KP_GL_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer); \
  KP_GL_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray); \
  KP_GL_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
  //KP_GL_PROC(PFNGL, gl);

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define KP_GL_PROC(type, name) extern type name
  KP_GL_PROCS
#undef KP_GL_PROC

#ifdef __cplusplus
}
#endif //__cplusplus
