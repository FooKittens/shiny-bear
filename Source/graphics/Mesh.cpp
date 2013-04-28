#include "graphics\Mesh.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"

namespace shinybear
{

Mesh::Mesh(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
  m_pVBuffer = nullptr;
  m_pIBuffer = nullptr;
  m_vertexCount = 0;
  m_indexCount = 0;
}

Mesh::~Mesh()
{
  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);
  m_vertices.clear();
  m_vertices.shrink_to_fit();
  m_indices.clear();
  m_indices.shrink_to_fit();
}

void Mesh::UpdateBuffers()
{
  if(!m_pVBuffer || m_vertexCount != m_vertices.size())
  {
    RELEASECOM(m_pVBuffer);
    HR(m_pProvider->GetDevice()->CreateVertexBuffer(
      sizeof(BlockVertex) * m_vertices.size(),
      D3DUSAGE_WRITEONLY,
      0,
      D3DPOOL_MANAGED,
      &m_pVBuffer,
      NULL
    ));
  }

  BlockVertex *pVertices;
  HR(m_pVBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0));

  for(int i = 0; i < m_vertices.size(); ++i)
  {
    pVertices[i] = m_vertices[i];
  }

  HR(m_pVBuffer->Unlock());

  m_vertices.clear();
  m_vertices.shrink_to_fit();

  if(!m_pIBuffer || m_indexCount != m_indices.size())
  {
    RELEASECOM(m_pIBuffer);
    HR(m_pProvider->GetDevice()->CreateIndexBuffer(
      sizeof(UINT) * m_indices.size(),
      D3DUSAGE_WRITEONLY,
      D3DFMT_INDEX32,
      D3DPOOL_MANAGED,
      &m_pIBuffer,
      NULL
    ));
  }

  UINT *pIndices;
  HR(m_pIBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0));

  for(int i = 0; i < m_indices.size(); ++i)
  {
    pIndices[i] = m_indices[i];
  }
  
  HR(m_pIBuffer->Unlock());

  m_indices.clear();
  m_indices.shrink_to_fit();
}

UINT Mesh::AddVertex(const D3DXVECTOR3 &pos, const BlockMaterial &mat,
  const D3DXVECTOR3 &normal)
{
  BlockVertex vert;
  vert.normal = normal;
  vert.position = pos;
  mat.GetColors(&vert.diffuse, &vert.specular);
  m_vertices.push_back(vert);
  ++m_vertexCount;
  return m_currentIndex++;
}

void Mesh::AddTriangle(UINT v1, UINT v2, UINT v3)
{
  m_indices.push_back(v1);
  m_indices.push_back(v2);
  m_indices.push_back(v3);
  m_indexCount += 3;
}

void Mesh::RenderMesh()
{
  m_pProvider->GetDevice()->SetIndices(m_pIBuffer);
  m_pProvider->GetDevice()->SetStreamSource(0, m_pVBuffer, 0, sizeof(BlockVertex));

  m_pProvider->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
    0, 0, m_vertexCount, 0, m_indexCount / 3);
}


} // namespace shinybear