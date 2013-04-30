#include "scene\CameraNode.h"

namespace shinybear
{

CameraNode::CameraNode(GameWindow *pWindow, SceneNode *pTarget)
{
  m_upVector = Vector3::kUnitY;
  m_position = Vector3::kZero;
  m_targetPosition = Vector3::kZero;

  m_pTarget = pTarget;
  SetFieldOfView(60.0f);
  SetAspectRatio(pWindow->GetSize());
  SetViewDistance(1000.0f);
  SetupProjection();

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
  if(m_pTarget)
  {
    m_targetPosition = m_pTarget->GetTransform().GetPosition();
  }

  Mat4x4 mat = m_pTarget->GetTransform();
  Vector4 target = Vector4::kUnitZ * -10.0f;
  target += Vector4::kUnitY * -10.0f;
  Vector4 targetWorld = mat.Transform(target);
  m_upVector = mat.Transform(Vector3::kUnitY);
  m_position = mat.GetPosition() + Vector3(targetWorld);
  

  //m_position = m_view.GetPosition();

  m_view = Mat4x4::CreateLookAt(m_position, m_targetPosition, m_upVector);

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
    m_fieldOfView * (D3DX_PI / 180.0f), 0.1f, m_viewDistance);
}


} // namespace shinybear