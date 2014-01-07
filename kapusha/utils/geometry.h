#pragma once
#include "../math.h"

#ifdef _MSC_VER
#pragma warning(1 : 4519)
#endif

namespace kapusha {
  using namespace math;
  //! function to calculate normals for a mesh with arbitrary vtx fmt in place
  //! \param *_stride is in float units
  //! \param indices_count must be > 2
  template <typename IndexType = u16>
  void calculateNormals(float *vertices_ptr_in, int vertices_stride,
                        float *normals_ptr_out, int normals_stride,
                        IndexType *indices, int indices_count);
 
  //! Function to calculate required amount of storage required for a plane.
  //! \param[in] cells Number of generated cells along the corresponding axis.
  //!            One cell is a quad of two triangles,
  //! \param[out] vertices Number of vertices that would be generated.
  //! \param[out] indices Number of indices that would be generated.
  void calculateXYPlaneStorage(vec2i cells, int &vertices, int &indices);
  
  //! Plane generator that generates a z=0 plane.
  //! Makes vertices for xy-plane in a unit rect and generates a triangle-strip
  //! indices that fill it.
  //! \param[in] cells Number of cells to generate
  //! \todo Document more stuffs.
  template <typename IndexType = u16>
  void makeXYPlane(vec2i cells,
                   float *vertices_ptr_in, int vertices_stride,
                   IndexType *indices, int indices_offset = 0);
} // namespace kapusha
