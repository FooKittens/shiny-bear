#include "util\math\Frustum.h"
#include "base\system\GraphicsProvider.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace shinybear
{


void Frustum::Create(float fov, float aspect, float nPlane, float fPlane)
{
  // Horizontal field of view.
  float fovH = atanf(tan(fov / 2.0f) * aspect) * 2.0f;

  float nearHeight = tanf(fov / 2.0f) * nPlane * 2.0f;
  float nearWidth = tan(fovH / 2.0f) * nPlane * 2.0f;
  float nearDepth = nPlane;

  // Top Left near point.
  m_nearPoints[0] = Vector3(-nearWidth / 2.0f, nearHeight / 2.0f, nearDepth);
  
  // Bottom left near point.
  m_nearPoints[1] = Vector3(-nearWidth / 2.0f, -nearHeight / 2.0f, nearDepth);

  // Bottom right near point.
  m_nearPoints[2] = Vector3(nearWidth / 2.0f, -nearHeight / 2.0f, nearDepth);

  // Top Right near point.
  m_nearPoints[3] = Vector3(nearWidth / 2.0f, nearHeight / 2.0f, nearDepth);


  float farHeight = tanf(fov / 2.0f) * fPlane * 2.0f;
  float farWidth = tanf(fovH / 2.0f) * fPlane * 2.0f;
  float farDepth = (fPlane);

  // Top Left far point.
  m_farPoints[0] = Vector3(-farWidth / 2.0f, farHeight / 2.0f, farDepth);

  // Top right far point.
  m_farPoints[1] = Vector3(farWidth / 2.0f, farHeight / 2.0f, farDepth);

  // Bottom right far point.
  m_farPoints[2] = Vector3(farWidth / 2.0f, -farHeight / 2.0f, farDepth);

  // Bottom left far point.
  m_farPoints[3] = Vector3(-farWidth / 2.0f, -farHeight / 2.0f, farDepth);


  // Near plane
  m_planes[0].Create(m_nearPoints[0], m_nearPoints[1], m_nearPoints[2]);

  // Far plane
  m_planes[1].Create(m_farPoints[0], m_farPoints[1], m_farPoints[2]);

  // Top plane
  m_planes[2].Create(m_farPoints[0], m_nearPoints[0], m_nearPoints[3]);

  // Bottom plane
  m_planes[3].Create(m_farPoints[3], m_farPoints[2], m_nearPoints[2]);

  // Left Plane
  m_planes[4].Create(m_nearPoints[0], m_farPoints[0], m_farPoints[3]);

  // Right plane
  m_planes[5].Create(m_farPoints[1], m_nearPoints[3], m_nearPoints[2]);
}


bool Frustum::IsInside(const Vector3 &point, float radius)
{
  for(UINT i = 0; i < kPlaneCount; ++i)
  {
    if(!m_planes[i].IsInside(point, radius))
    {
      return false;
    }
  }

  return true;
}

struct CVertex
{
  CVertex(const Vector3 &v, UINT col)
    :x(v.x), y(v.y), z(v.z), color(col) { }
  float x, y, z;
  D3DCOLOR color;
};
const DWORD CVertexFVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

void Frustum::Render(GraphicsProvider *pProvider) const
{
  IDirect3DDevice9 *pDevice = pProvider->GetDevice();

  CVertex vertices[] = 
  {
    // Top Near line
    CVertex(m_nearPoints[0], 0xFFFF0000),
    CVertex(m_nearPoints[3], 0xFFFF0000 ),

    // Right Near line
    CVertex(m_nearPoints[3], 0xFFFF0000 ),
    CVertex(m_nearPoints[2], 0xFFFF0000 ),

    // Bottom near line
    CVertex(m_nearPoints[2], 0xFFFF0000 ),
    CVertex(m_nearPoints[1], 0xFFFF0000 ),

    // Left Near line
    CVertex(m_nearPoints[1], 0xFFFF0000 ),
    CVertex(m_nearPoints[0], 0xFFFF0000 ),

    // Top far line
    CVertex(m_farPoints[0], 0xFF66FF55 ),
    CVertex(m_farPoints[1], 0xFF66FF55 ),

    // Right far line
    CVertex(m_farPoints[1], 0xFF66FF55 ),
    CVertex(m_farPoints[2], 0xFF66FF55 ),

    // Bottom far line
    CVertex(m_farPoints[2], 0xFF66FF55 ),
    CVertex(m_farPoints[3], 0xFF66FF55 ),

    // Left far line
    CVertex(m_farPoints[3], 0xFF66FF55 ),
    CVertex(m_farPoints[0], 0xFF66FF55 ),

    // Top Right Near To Far
    CVertex(m_nearPoints[3], 0xFF667755 ),
    CVertex(m_farPoints[1], 0xFF667755 ),

    // Bottom Right near To Far
    CVertex(m_nearPoints[2], 0xFF667755 ),
    CVertex(m_farPoints[2], 0xFF667755 ),

    // Top Left near to far
    CVertex(m_nearPoints[0], 0xFF667755 ),
    CVertex(m_farPoints[0], 0xFF667755 ),

    // Bottom Left near to far
    CVertex(m_nearPoints[1], 0xFF667755 ),
    CVertex(m_farPoints[3], 0xFF667755),
  };


  UINT primCount = sizeof(vertices) / sizeof(CVertex) / 2;
  pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
  //pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  //pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  //pDevice->SetVertexDeclaration(nullptr);
  //pDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
  pDevice->SetFVF(CVertexFVF);
  pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 12,
    vertices, sizeof(CVertex));

  pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
  //pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  //pDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
}


} // namespace shinybear
