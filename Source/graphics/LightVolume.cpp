#include "graphics\LightVolume.h"
#include "base\System\GraphicsProvider.h"
#include "util\SBUtil.h"
#include <cassert>

namespace shinybear
{

LightVolume::LightVolume(GraphicsProvider *pProvider)
  :m_pProvider(pProvider)
{
  m_pVBuffer = nullptr;
  m_pIBuffer = nullptr;
}

LightVolume::~LightVolume()
{
  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);
}

void LightVolume::Render()
{
  if(m_pVBuffer && m_pIBuffer)
  {
    IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

    HR(pDevice->SetIndices(m_pIBuffer));
    HR(pDevice->SetStreamSource(0, m_pVBuffer, 0, sizeof(VertexData)));
    HR(pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
      m_vertexCount, 0, m_indexCount / 3));
  }
}

void LightVolume::MakeQuad()
{
  static const int kVertexCount = 4;
  static const int kIndexCount = 6;

  m_indexCount = kIndexCount;
  m_vertexCount = kVertexCount;

  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);

  VertexData vertices[] = 
  {
    { Vector3(-0.5f, +0.5f, 0) },
    { Vector3(+0.5f, +0.5f, 0) },
    { Vector3(+0.5f, -0.5f, 0) },
    { Vector3(-0.5f, -0.5f, 0) },
  };

  UINT indices[] =
  {
    0, 1, 2,
    2, 3, 0
  };

  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();
  
  HR(pDevice->CreateVertexBuffer(
    sizeof(VertexData) * kVertexCount,
    D3DUSAGE_WRITEONLY,
    0,
    D3DPOOL_MANAGED,
    &m_pVBuffer,
    NULL
  ));

  VertexData *pData;
  HR(m_pVBuffer->Lock(0, 0, reinterpret_cast<void**>(&pData), 0));

  for(int i = 0; i < kVertexCount; ++i)
  {
    pData[i] = vertices[i];
  }

  HR(m_pVBuffer->Unlock());

  HR(pDevice->CreateIndexBuffer(
    sizeof(UINT) * kIndexCount,
    D3DUSAGE_WRITEONLY,
    D3DFMT_INDEX32,
    D3DPOOL_MANAGED,
    &m_pIBuffer,
    NULL
  ));

  UINT *pIData;
  HR(m_pIBuffer->Lock(0, 0, reinterpret_cast<void**>(&pIData), 0));

  for(int i = 0; i < kIndexCount; ++i)
  {
    pIData[i] = indices[i];
  }

  HR(m_pIBuffer->Unlock());

  m_type = LV_QUAD;
}

void LightVolume::MakeSphere()
{
  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);

  UINT kVertexCount = 8;
  UINT kIndexCount = 36;

  VertexData vertices[] = 
  {
    Vector3(-0.5f, +0.5f, +0.5f),
    Vector3(+0.5f, +0.5f, +0.5f),
    Vector3(+0.5f, -0.5f, +0.5f),
    Vector3(-0.5f, -0.5f, +0.5f),

    Vector3(+0.5f, +0.5f, -0.5f),
    Vector3(-0.5f, +0.5f, -0.5f),
    Vector3(-0.5f, -0.5f, -0.5f),
    Vector3(+0.5f, -0.5f, -0.5f)
  };

  UINT indexData[] = 
  {
    // Front
    0, 1, 2,
    2, 3, 0,

    // Back
    4, 5, 6,
    6, 7, 4,

    // Top
    5, 4, 1,
    1, 0, 5,

    // Bottom
    3, 2, 7,
    7, 6, 3,

    // Left
    5, 0, 3,
    3, 6, 5,

    // Right,
    1, 4, 7,
    7, 2, 1
  };

  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  m_vertexCount = 8;

  HR(pDevice->CreateVertexBuffer(
    sizeof(VertexData) * m_vertexCount,
    D3DUSAGE_WRITEONLY,
    0,
    D3DPOOL_MANAGED,
    &m_pVBuffer,
    NULL
  ));

  VertexData *pVerts;
  HR(m_pVBuffer->Lock(0, 0, (void**)(&pVerts), 0));

  for(int i = 0; i < m_vertexCount; ++i)
  {
    pVerts[i] = vertices[i];
  }

  HR(m_pVBuffer->Unlock());

  m_indexCount = 36;

  HR(pDevice->CreateIndexBuffer(
    sizeof(UINT) * m_indexCount,
    D3DUSAGE_WRITEONLY,
    D3DFMT_INDEX32,
    D3DPOOL_MANAGED,
    &m_pIBuffer,
    NULL
  ));

  UINT *pIndices;
  HR(m_pIBuffer->Lock(0, 0, (void**)(&pIndices), 0));

  for(int i = 0; i < m_indexCount; ++i)
  {
    pIndices[i] = indexData[i];
  }

  HR(m_pIBuffer->Unlock());

  
  m_type = LV_SPHERE;
}

void LightVolume::MakeCone()
{

}

void LightVolume::OnDeviceReset()
{
  switch(m_type)
  {
  case LV_QUAD:
    MakeQuad();
    break;
  case LV_SPHERE:
    MakeSphere();
    break;
  case LV_CONE:
    MakeCone();
    break;
  default:
    assert(false && "Invalid LightVolume type!");
  }
}

void LightVolume::OnDeviceLost()
{
  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);
}

} // namespace shinybear