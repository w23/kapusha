#include "../core.h"
#include "geometry.h"

namespace kapusha {
  template <typename IndexType>
  void calculateNormals(float *vertices_ptr_in, int vertices_stride,
                        float *normals_ptr_out, int normals_stride,
                        IndexType *indices, int indices_count) {
    KP_ASSERT(indices_count > 2);
    // 1st pass -- zero all normals
    for (int i = 0; i < indices_count; ++i) {
      int j = indices[i] * normals_stride;
      normals_ptr_out[j] = normals_ptr_out[j+1] = normals_ptr_out[j+2] = 0;
    }
    // 2nd pass -- add triangles' normals to its vertices
    for (int i = 0; i < indices_count-2; ++i) {
      unsigned idx[3] = { indices[i], indices[i+1], indices[i+2] };
      // skip degenerate triangles
      if (idx[0] == idx[1] || idx[1] == idx[2] || idx[2] == idx[0]) continue;
      if (i&1) std::swap(idx[0], idx[1]);
      // calc triangle normal
      vec3f v0(vertices_ptr_in + idx[0]*vertices_stride),
            v1(vertices_ptr_in + idx[1]*vertices_stride),
            v2(vertices_ptr_in + idx[2]*vertices_stride);
      vec3f n(((v1-v0).cross(v2-v0)).normalized());
      // add the normal to all vertices
#define _NORMALSUM(i) \
      { float *np = normals_ptr_out + idx[i]*normals_stride; \
        vec3f N(vec3f(np) + n); np[0] = N.x; np[1] = N.y; np[2] = N.z; }
      _NORMALSUM(0);
      _NORMALSUM(1);
      _NORMALSUM(2);
#undef _NORMALSUM
    }
    // 3rd pass -- normalize all normals
    for (int i = 0; i < indices_count; ++i) {
      float *np = normals_ptr_out + indices[i] * normals_stride;
      vec3f n(vec3f(np).normalized());
      np[0] = n.x; np[1] = n.y; np[2] = n.z;
    }
  }
  
  void calculateXYPlaneStorage(vec2i cells, int &vertices, int &indices) {
    vertices = (cells+vec2i(1)).product();
    indices = (cells.x + 2) * cells.y * 2 - 2;
  }
  
  template <typename IndexType>
  void makeXYPlane(vec2i cells,
                   float *vertices_ptr_in, int vertices_stride,
                   IndexType *indices, int indices_offset) {
    vec2i samples(cells+vec2i(1));
    float *pv = vertices_ptr_in;
    for (int y = 0; y < samples.y; ++y) {
      float Y = static_cast<float>(y) / static_cast<float>(samples.y);
      for (int x = 0; x < samples.x; ++x, pv += vertices_stride)
        pv[0] = static_cast<float>(x) / static_cast<float>(samples.x), pv[1] = Y;
    }
    IndexType *pi = indices;
    int index = indices_offset;
    for (int y = 0; y < cells.y; ++y) {
      for (int x = 0; x < samples.x; ++x, ++index) {
        *pi++ = index + samples.x;
        *pi++ = index;
      }
      *pi++ = index - 1;
      *pi++ = index + samples.x;
    }
  }
  
  // declare templates
#define DECLARE(type) template \
  void calculateNormals<type>(float *vertices_ptr_in, int vertices_stride, \
                             float *normals_ptr_out, int normals_stride, \
                             type *indices, int indices_count)
  DECLARE(u8); DECLARE(u16); DECLARE(u32);
#undef DECLARE
#define DECLARE(type) template \
  void makeXYPlane<type>(vec2i cells, \
                        float *vertices_ptr_in, int vertices_stride, \
                        type *indices, int indices_offset)
  DECLARE(u8); DECLARE(u16); DECLARE(u32);
} // namespace kapusha