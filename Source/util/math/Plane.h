#ifndef SHINYBEAR_PLANE_H
#define SHINYBEAR_PLANE_H

#include "util\math\Math.h"
#include <d3dx9.h>


namespace shinybear
{

class Plane : D3DXPLANE
{
public:
  Plane(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2)
  {
    Create(p0, p1, p2);
  }
  Plane() { }

  void Normalize();
  bool IsInside(const Vector3 &point, const float radius = 0.0f) const;

  void Create(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2);
};

inline void Plane::Normalize()
{
  float len = sqrt(a * a + b * b + c * c);
  a /= len;
  b /= len;
  c /= len;
  d /= len;
}

inline void Plane::Create(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2)
{
   D3DXPlaneFromPoints(this, &p0, &p1, &p2);
}

inline bool Plane::IsInside(const Vector3 &point, const float radius) const
{
  return D3DXPlaneDotCoord(this, &point) >= -radius;
}

} // namespace shinybear

#endif // SHINYBEAR_PLANE_H