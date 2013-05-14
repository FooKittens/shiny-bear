#include "scene\camera\Camera.h"
#include "base\system\GraphicsProvider.h"


namespace shinybear
{

Camera::Camera(float aspectRatio, float fov, float znear, float zfar)
  :m_speed(8.0f)
{
  SetViewDistance(zfar);
  SetNearPlane(znear);
  SetFieldOfView(fov);
  SetAspectRatio(aspectRatio);

  UpdateViewMatrix();
  UpdateProjectionMatrix();
  m_worldPosition = Vector3::kZero;
}

Camera::~Camera()
{

}

void Camera::Update(double dt)
{
  if(m_isDebug)
  {
    DoFreeCamMovement(dt);
  }

  UpdateViewMatrix();
}

float rotY = 0.0f;
void Camera::DoFreeCamMovement(double dt)
{
  InputManager::GetKeyboardState(&m_keys);
  InputManager::GetMouseState(&m_mouse);

  Vector3 deltaX = Vector3::kZero;
  Vector3 deltaZ = Vector3::kZero;
  Vector3 deltaY = Vector3::kZero;
  // Velocity with time included.
  float v = m_speed * dt;

  if(m_keys.IsKeyDown(Keys::K_A))
  {
    deltaX -= Vector3::kUnitX * v;
  }
  
  if(m_keys.IsKeyDown(Keys::K_D))
  {
    deltaX += Vector3::kUnitX * v;
  }

  if(m_keys.IsKeyDown(Keys::K_W))
  {
    deltaZ += Vector3::kUnitZ * v;
  }

  if(m_keys.IsKeyDown(Keys::K_S))
  {
    deltaZ -= Vector3::kUnitZ * v;
  }

  if(m_keys.IsKeyDown(Keys::K_UP))
  {
    deltaY += Vector3::kUnitY * v;
  }

  if(m_keys.IsKeyDown(Keys::K_DOWN))
  {
    deltaY -= Vector3::kUnitY * v;
  }
  
  if(m_keys.IsKeyDown(Keys::K_LEFT))
  {
    rotY -= 1.0f * v / 2.0f;
  }

  if(m_keys.IsKeyDown(Keys::K_RIGHT))
  {
    rotY += 1.0f * v / 2.0f;
  }

  m_worldPosition += deltaX + deltaZ + deltaY;
  m_forwardVector = Mat4x4::CreateRotationY(rotY).Transform(Vector3::kUnitZ);
  m_upVector = Vector3::kUnitY;
}

void Camera::UpdateViewMatrix()
{
  Vector3 target = m_worldPosition + m_forwardVector;

  D3DXMatrixLookAtLH(&m_view, &m_worldPosition, &target, &m_upVector);
}

void Camera::UpdateProjectionMatrix()
{
  m_frustum.Create(m_fov * (D3DX_PI / 180.0f), m_aspect, m_near, m_far);
  D3DXMatrixPerspectiveFovLH(&m_projection, m_fov * (D3DX_PI / 180.0f),
    m_aspect, m_near, m_far);
}

void Camera::RenderFrustum(GraphicsProvider *pProvider) const
{
  
  float angle;
  Vector3 axis;
  Quaternion::CreateFromMatrix(m_view).GetAxisAngle(&axis, &angle);

  Mat4x4 world = Mat4x4::CreateAxisAngle(axis, -angle) * 
    Mat4x4::CreateTranslation(m_worldPosition);
  
  IDirect3DDevice9 *pDevice = pProvider->GetDevice();
  pDevice->SetTransform(D3DTS_WORLD, &Mat4x4::kIdentity);
  pDevice->SetTransform(D3DTS_VIEW, &m_view);
  pDevice->SetTransform(D3DTS_PROJECTION, &m_projection);

  m_frustum.Render(pProvider);
}



} // namespace shinybear