#include "graphics\LightVolume.h"
#include "base\System\GraphicsProvider.h"
#include "util\SBUtil.h"
#include <cassert>
#include <vector>

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


UINT LightVolume::SplitEdge(std::vector<Vector3> &verts,
  const Vector3 &v0, const Vector3 &v1)
{
  Vector3 v = (v0 + v1) / 2.0f;
  verts.push_back(v);
  return verts.size() - 1;
}

void LightVolume::MakeSphere()
{
  RELEASECOM(m_pVBuffer);
  RELEASECOM(m_pIBuffer);

  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  std::vector<Vector3> verts;
  std::vector<Triangle> tris;

  CreateSphereData(1, verts, tris);

  m_vertexCount = verts.size();

  HR(pDevice->CreateVertexBuffer(
    sizeof(VertexData) * m_vertexCount,
    D3DUSAGE_WRITEONLY,
    0,
    D3DPOOL_MANAGED,
    &m_pVBuffer,
    NULL
  ));

  VertexData *pVertices;
  HR(m_pVBuffer->Lock(0, 0, (void**)(&pVertices), 0));

  int vCount = 0;
  for(int i = 0; i < verts.size(); ++i)
  {
    verts[i].Normalize();
    pVertices[i].position = verts[i];
  }

  HR(m_pVBuffer->Unlock());

  
  m_indexCount = tris.size() * 3;
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

  int iCount = 0;
  for(int i = 0; i < tris.size(); ++i)
  {
    pIndices[iCount++] = tris[i].v0;
    pIndices[iCount++] = tris[i].v1;
    pIndices[iCount++] = tris[i].v2;
  }

  HR(m_pIBuffer->Unlock());
  
  m_type = LV_SPHERE;
}

void LightVolume::CreateSphereData(UINT levels, std::vector<Vector3> &verts,
  std::vector<Triangle> &tris)
{
  verts.clear();
  verts.shrink_to_fit();
  tris.clear();
  tris.shrink_to_fit();

  float t = (1.0f + sqrtf(5.0f)) / 2.0f;

  float l = 1.0f;

  // X-Parallell
  verts.push_back(Vector3(-t, +l, +0)); // p7
  verts.push_back(Vector3(+t, +l, +0)); // p5
  verts.push_back(Vector3(+t, -l, +0)); // p4
  verts.push_back(Vector3(-t, -l, +0)); // p6  ---  3

  // Y-Parellell
  verts.push_back(Vector3(+0, +t, -l)); // p0
  verts.push_back(Vector3(+0, +t, +l)); // p1
  verts.push_back(Vector3(+0, -t, +l)); // p3
  verts.push_back(Vector3(+0, -t, -l)); // p2 ----  7

  // Z-ortho Top Right.
  verts.push_back(Vector3(-l, +0, +t)); // p8
  verts.push_back(Vector3(+l, +0, +t)); // p9
  verts.push_back(Vector3(+l, +0, -t)); // p11
  verts.push_back(Vector3(-l, +0, -t)); // p10 --- 11

  // 5 Triangles around 0
  tris.push_back(Triangle(0, 5, 4));
  tris.push_back(Triangle(0, 4, 11));
  tris.push_back(Triangle(0, 8, 5));
  tris.push_back(Triangle(0, 3, 8));
  tris.push_back(Triangle(0, 11, 3));

  tris.push_back(Triangle(4, 1, 10));
  tris.push_back(Triangle(4, 5, 1));
  tris.push_back(Triangle(1, 5, 9));
  tris.push_back(Triangle(5, 8, 9));
  tris.push_back(Triangle(9, 8, 6));
  tris.push_back(Triangle(8, 3, 6));
  tris.push_back(Triangle(7, 6, 3));
  tris.push_back(Triangle(3, 11, 7));
  tris.push_back(Triangle(11, 10, 7));
  tris.push_back(Triangle(11, 4, 10));

  tris.push_back(Triangle(2, 10, 1));
  tris.push_back(Triangle(2, 1, 9));
  tris.push_back(Triangle(2, 9, 6));
  tris.push_back(Triangle(2, 6, 7));
  tris.push_back(Triangle(2, 7, 10));

  for(UINT i = 0; i < levels; ++i)
  {
    std::vector<Triangle> nTris;
    for(UINT ti = 0; ti < tris.size(); ++ti)
    {
      Triangle t = tris[ti];
      UINT v0 = SplitEdge(verts, verts[tris[ti].v0], verts[tris[ti].v1]);
      UINT v1 = SplitEdge(verts, verts[tris[ti].v1], verts[tris[ti].v2]);
      UINT v2 = SplitEdge(verts, verts[tris[ti].v0], verts[tris[ti].v2]);

      nTris.push_back(Triangle(t.v0, v0, v2));
      nTris.push_back(Triangle(t.v1, v1, v0));
      nTris.push_back(Triangle(t.v2, v2, v1));
      nTris.push_back(Triangle(v0, v1, v2));
    }

    tris.swap(nTris);
  }
}

void LightVolume::MakeCone()
{

}

void LightVolume::OnDeviceReset(GraphicsProvider *pProvider)
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