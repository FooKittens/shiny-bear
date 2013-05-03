#ifndef SHINYBEAR_MESH_H
#define SHINYBEAR_MESH_H

#include <d3d9.h>
#include <vector>
#include "graphics\VertexFormats.h"
#include "util\Typedefs.h"

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class Mesh
{
public:
  Mesh(GraphicsProvider *pProvider);
  ~Mesh();

  void Reserve(UINT vcount);

  void BeginMesh();
  void EndMesh();

  UINT AddVertex(const D3DXVECTOR3 &position, const BlockMaterial &mat,
    const D3DXVECTOR3 &normal);
  void AddTriangle(UINT v1, UINT v2, UINT v3);

  void UpdateBuffers();

  void RenderMesh();

private:
  void Clear();

  GraphicsProvider *m_pProvider;
  size_t m_vertexCount;
  size_t m_indexCount;
  size_t m_currentIndex;
  bool m_isEditing;

  std::vector<BlockVertex> m_vertices;
  std::vector<UINT> m_indices;
  IDirect3DVertexBuffer9 *m_pVBuffer;
  IDirect3DIndexBuffer9 *m_pIBuffer;
};

inline void Mesh::Reserve(UINT vcount)
{
  m_vertices.reserve(vcount);
  m_indices.reserve(vcount * 3);
}

} // namespace shinybear

#endif // SHINYBEAR_MESH_H