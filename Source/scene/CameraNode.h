#ifndef SHINYBEAR_CAMERANODE_H
#define SHINYBEAR_CAMERANODE_H

#include "scene\SceneNode.h"
#include "base\system\GameWindow.h"
#include "util\math\Math.h"

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

  const Mat4x4 &GetViewMatrix() const;
  const Mat4x4 &GetProjectionMatrix() const;

  void Update(double elapsedSeconds);
  void Render(SceneManager *pScene);

protected:
  virtual void SetupProjection();

private:
  SceneNode *m_pTarget;
  Mat4x4 m_view;
  Mat4x4 m_projection;
  Vector3 m_position;
  Vector3 m_targetPosition;
  Vector3 m_upVector;

  bool m_displayFrustum;
  float m_fieldOfView;
  float m_aspectRatio;
  float m_viewDistance;
  
};

inline const Mat4x4 &CameraNode::GetViewMatrix() const
{
  return m_view;
}

inline const Mat4x4 &CameraNode::GetProjectionMatrix() const
{
  return m_projection;
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