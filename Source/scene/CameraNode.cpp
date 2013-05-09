#include "scene\CameraNode.h"

namespace shinybear
{

CameraNode::CameraNode(GameWindow *pWindow, SceneNode *pTarget)
{
  m_upVector = Vector3::kUnitY;
  m_position = Vector3::kZero;
  m_targetPosition = Vector3::kZero;
  m_followSpeed = 10.0f;
  m_pTarget = pTarget;
  SetFieldOfView(66.77790f);
  SetAspectRatio(pWindow->GetSize());
  SetViewDistance(500.0f);
  SetNearPlane(1.0f);
  SetupProjection();
  SetFreeCam(true);
  
  
  if(m_pTarget == nullptr)
  {
    m_targetPosition.x = 0;
    m_targetPosition.y = 0;
    m_targetPosition.z = 0;
  }
  
}

CameraNode::~CameraNode()
{

}

void CameraNode::Update(double elapsedSeconds)
{
  if(m_pTarget && !m_isFreeCam)
  {
    Mat4x4 tTrans = m_pTarget->GetTransform();
    m_targetPosition = tTrans.GetPosition();
    Vector4 target = -Vector4::kUnitZ * 10.0f;
    target += Vector4::kUnitY * 5.0f;
    target = tTrans.Transform(target);
    Vector3 tPos = Vector3(m_targetPosition.x, m_targetPosition.y, m_targetPosition.z);
    
    Vector3 diff = (Vector3(target) + tPos) - m_position;

    Vector3 dir = Vector3::Normalize(diff);

    Mat4x4 myTransform = GetTransform().Inverse().Transpose();
    Vector3 mf = GetTransform().Transform(Vector3::kUnitZ);

    dir *= m_followSpeed * elapsedSeconds;
    if(dir.Length() < diff.Length())
    {
      Translate(dir.x, dir.y, dir.z);
    }
    else
    {
      Translate(diff.x, diff.y, diff.z);
    }
    
    m_position = GetTransform().GetPosition();
    m_upVector = myTransform.Transform(Vector3::kUnitY);
  }
  else
  {
    m_position = GetTransform().GetPosition();

    Mat4x4 transform = GetTransform().Inverse().Transpose();
    m_targetPosition = m_position + transform.Transform(Vector3::kUnitZ);
    m_upVector = transform.Transform(Vector3::kUnitY);    
  }

  //m_position = m_view.GetPosition();

  m_view = Mat4x4::CreateLookAt(m_position, m_targetPosition, m_upVector);
  //m_view = GetTransform();
  SceneNode::Update(elapsedSeconds);
}

void CameraNode::Render(SceneManager *pScene)
{
  SceneNode::Render(pScene);
}

void CameraNode::OnResolutionChange(const Size &newSize)
{
  SetAspectRatio(newSize);
  SetupProjection();
}


void CameraNode::SetupProjection()
{
  m_projection = Mat4x4::CreatePerspectiveFovLH(m_aspectRatio,
    m_fieldOfView * (D3DX_PI / 180.0f), m_near, m_viewDistance);
}


} // namespace shinybear