#ifndef SHINYBEAR_CAMERA_H
#define SHINYBEAR_CAMERANODE_H

#include "scene\SceneNode.h"
#include "base\system\GameWindow.h"

namespace shinybear
{

class CameraNode : public SceneNode
{
public:
  CameraNode(GameWindow *pWindow, SceneNode *pTarget);
  ~CameraNode();

  virtual void OnResolutionChange(const Size &newSize);

  virtual void SetFieldOfView(float fovRads);
  virtual void SetAspectRatio(float aspect);
  virtual void SetAspectRatio(const Size &newSize);
  virtual void SetViewDistance(float distance);

  const D3DXMATRIX *GetViewMatrix() const;
  const D3DXMATRIX *GetProjectionMatrix() const;

  void Update(double elapsedSeconds);
  void Render(SceneManager *pScene);

protected:
  virtual void SetupProjection();

private:
  SceneNode *m_pTarget;
  D3DXMATRIX m_view;
  D3DXMATRIX m_projection;
  bool m_displayFrustum;
  float m_fieldOfView;
  float m_aspectRatio;
  float m_viewDistance;
  D3DXVECTOR3 m_position;
  D3DXVECTOR3 m_targetPosition;
  D3DXVECTOR3 m_upVector;
  D3DXVECTOR3 m_zero;
};

inline const D3DXMATRIX *CameraNode::GetViewMatrix() const
{
  return &m_view;
}

inline const D3DXMATRIX *CameraNode::GetProjectionMatrix() const
{
  return &m_projection;
}

inline void CameraNode::SetFieldOfView(float fov)
{
  m_fieldOfView = fov;
}

inline void CameraNode::SetViewDistance(float distance)
{
  m_viewDistance = distance;
}

inline void CameraNode::SetAspectRatio(float aspect)
{
  m_aspectRatio = aspect;
}

inline void CameraNode::SetAspectRatio(const Size &size)
{
  SetAspectRatio((float)size.width / (float)size.height);
}

} // namespace shinybear

#endif // SHINYBEAR_CAMERANODE_H