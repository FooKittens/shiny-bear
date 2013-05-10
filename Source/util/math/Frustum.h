#ifndef SHINYBEAR_FRUSTUM_H
#define SHINYBEAR_FRUSTUM_H

#include "util\math\Plane.h"
#include "util\math\Math.h"

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class Frustum
{
public:
  Frustum(float fov, float aspect, float znear, float zfar)
  {
    Create(fov, aspect, znear, zfar);
  }
  Frustum() { }
  ~Frustum() { }

  void Create(float fov, float aspect, float znear, float zfar);
  bool IsInside(const Vector3 &point, float radius = 0.0f);

  void Render(GraphicsProvider *pProvider) const;

private:
  static const UINT kPlaneCount = 6;

  float m_fov;
  float m_aspect;
  float m_near;
  float m_far;

  Vector3 m_nearPoints[4];
  Vector3 m_farPoints[4];
  Plane m_planes[kPlaneCount];

};

} // namespace shinybear

#endif // SHINYBEAR_FRUSTUM_H