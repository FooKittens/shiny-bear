#include "scene\CameraNode.h"

namespace shinybear
{

CameraNode::CameraNode(GameWindow *pWindow, SceneNode *pTarget)
  :m_upVector(0, 1.0f, 0), m_position(0, 0, 0)
{
  m_pTarget = pTarget;
  m_fieldOfView = 0.0f;
  m_aspectRatio = 0.0f;
}

CameraNode::~CameraNode()
{

}

void CameraNode::Update(double elapsedSeconds)
{
  if(m_pTarget)
  {
    
  }

  D3DXMATRIX inv;
  float det = D3DXMatrixDeterminant(&m_view);
  D3DXMatrixInverse(&inv, &det, &m_view);

  D3DXVECTOR4 outp;

  D3DXVec3Transform(&outp, &m_zero, &inv);
}

void CameraNode::Render(SceneManager *pScene)
{

}

void CameraNode::OnResolutionChange(const Size &newSize)
{
  SetAspectRatio(newSize);
  SetupProjection();
}


void CameraNode::SetupProjection()
{
  D3DXMatrixPerspectiveFovLH(&m_projection,
    m_fieldOfView, m_aspectRatio, 0.1f, m_viewDistance);
}


} // namespace shinybear