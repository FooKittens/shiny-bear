#include "graphics\Mesh.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"
#include <cassert>

namespace shinybear
{

Mesh::Mesh(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
  m_pVBuffer = nullptr;
  m_pIBuffer = nullptr;
  m_vertexCount = 0;
  m_indexCount = 0;
  m_currentIndex = 0;
  m_isEditing = false;
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
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);

  if(m_vertexCount == 0 || m_indexCount == 0)
    return;
  
  HR(m_pProvider->GetDevice()->CreateVertexBuffer(
    sizeof(BlockVertex) * m_vertexCount,
    D3DUSAGE_WRITEONLY,
    0,
    D3DPOOL_MANAGED,
    &m_pVBuffer,
    NULL
  ));
  
  
  BlockVertex *pVertices;
  HR(m_pVBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0));

  for(UINT i = 0; i < m_vertexCount; ++i)
  {
    pVertices[i] = m_vertices[i];
  }

  HR(m_pVBuffer->Unlock());

  
  HR(m_pProvider->GetDevice()->CreateIndexBuffer(
    sizeof(UINT) * m_indexCount,
    D3DUSAGE_WRITEONLY,
    D3DFMT_INDEX32,
    D3DPOOL_MANAGED,
    &m_pIBuffer,
    NULL
  ));
  

  UINT *pIndices;
  HR(m_pIBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0));

  for(UINT i = 0; i < m_indexCount; ++i)
  {
    pIndices[i] = m_indices[i];
  }
  
  HR(m_pIBuffer->Unlock());
}

UINT Mesh::AddVertex(const D3DXVECTOR3 &pos, const BlockMaterial &mat,
  const D3DXVECTOR3 &normal)
{
  BlockVertex vert;
  vert.normal = normal;
  vert.position = pos;
  mat.GetColors(&vert.diffuse, &vert.specular);
  m_vertices.push_back(vert);
  return m_currentIndex++;
}

void Mesh::AddTriangle(UINT v1, UINT v2, UINT v3)
{
  m_indices.push_back(v1);
  m_indices.push_back(v2);
  m_indices.push_back(v3);
}

void Mesh::RenderMesh()
{
  if(m_pIBuffer && m_pVBuffer)
  {
    HR(m_pProvider->GetDevice()->SetIndices(m_pIBuffer));
    HR(m_pProvider->GetDevice()->SetStreamSource(0, m_pVBuffer, 0, sizeof(BlockVertex)));

    HR(m_pProvider->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
      0, 0, m_vertexCount, 0, m_indexCount / 3));
  }
  else
  {
    //assert(false && "No vertex buffers in mesh!");
  }
}


void Mesh::BeginMesh()
{
  assert(m_isEditing == false && "Mesh is already in edit state!");
  Clear();
  m_isEditing = true;
}

void Mesh::EndMesh()
{
  assert(m_isEditing == true && "Mesh is not being edited!");

  m_vertexCount = m_vertices.size();
  m_indexCount = m_indices.size();

  UpdateBuffers();
  m_currentIndex = 0;

  m_isEditing = false;
}

void Mesh::Clear()
{
  m_vertexCount = 0;
  m_indexCount = 0;
  m_currentIndex = 0;

  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);

  m_vertices.clear();
  m_indices.clear();
}


} // namespace shinybear