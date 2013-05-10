#ifndef SHINYBEAR_CAMERA_H
#define SHINYBEAR_CAMERA_H

#include "util\math\Math.h"
#include "util\math\Frustum.h"
#include "util\input\InputManager.h"
#include "base\system\GameWindow.h"

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class Camera
{
public:
  Camera(float aspectRatio, float fov, float znear, float zfar);
  virtual ~Camera();

  // Allows the camera to reposition itself.
  virtual void Update(double elapsedSeconds);

  // As far as the camera can see, bounds the view frustum.
  virtual void SetViewDistance(float farPlane);
  virtual float GetViewDistance() const;

  // Anything closer to the camera than this value will be clipped.
  virtual void SetNearPlane(float nearPlane);
  virtual float GetNearPlane() const;

  // Used to calculate projection matrix.
  virtual void SetFieldOfView(float degrees);
  virtual float GetFieldOfView() const;
  
  virtual void SetAspectRatio(float aspect);
  virtual void SetAspectRatio(const Size &newSize);
  virtual float GetAspectRatio() const;

  virtual const Mat4x4 &GetViewMatrix() const;
  virtual const Mat4x4 &GetProjectionMatrix() const;

  // When debug mode is enabled, the camera is detached from its view collision
  // frustum, which allows us to debug clipping.
  virtual bool IsDebugMode() const;
  virtual void SetDebugMode(bool debug);

  virtual const Frustum &GetFrustum() const;

  virtual void RenderFrustum(GraphicsProvider *pProvider) const;

protected:
  virtual void UpdateProjectionMatrix();
  virtual void UpdateViewMatrix();

private:
  void DoFreeCamMovement(double dt);

  Mat4x4 m_view;
  Mat4x4 m_projection;
  Frustum m_frustum;

  float m_fov;
  float m_aspect;
  float m_near;
  float m_far;
  
  Vector3 m_worldPosition;
  Vector3 m_forwardVector;
  Vector3 m_upVector;

  bool m_isDebug;

  // For free cam movement.
  KeyboardState m_keys;
  MouseState m_mouse;
  const float m_speed;
};

inline const Frustum &Camera::GetFrustum() const
{
  return m_frustum;
}

inline void Camera::SetViewDistance(float zfar)
{
  m_far = zfar;
  //UpdateProjectionMatrix();
}
inline float Camera::GetViewDistance() const { return m_far; }

inline void Camera::SetNearPlane(float znear)
{ 
  m_near = znear;
  //UpdateProjectionMatrix();
}
inline float Camera::GetNearPlane() const { return m_near; }

inline void Camera::SetFieldOfView(float deg)
{ 
  m_fov = deg;
  //UpdateProjectionMatrix();
}
inline float Camera::GetFieldOfView() const { return m_fov; }

inline void Camera::SetAspectRatio(float aspect)
{
  m_aspect = aspect;
  //UpdateProjectionMatrix();
}
inline void Camera::SetAspectRatio(const Size &sz)
{
  m_aspect = static_cast<float>(sz.width) / static_cast<float>(sz.height);
  //UpdateProjectionMatrix();
}
inline float Camera::GetAspectRatio() const { return m_aspect; }

inline const Mat4x4 &Camera::GetViewMatrix() const { return m_view; }
inline const Mat4x4 &Camera::GetProjectionMatrix() const { return m_projection; }

inline bool Camera::IsDebugMode() const { return m_isDebug; }
inline void Camera::SetDebugMode(bool v) { m_isDebug = v; }
} // namespace shinybear

#endif // CAMERA_H