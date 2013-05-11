#ifndef SHINYBEAR_IDRAWABLE_H
#define SHINYBEAR_IDRAWABLE_H

#include "util\math\Math.h"

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{
class IDrawable
{
public:
  virtual const Mat4x4 &GetWorldMatrix();
  virtual float GetBoundingRadius() = 0;
  virtual void Render(GraphicsProvider *pProvider) = 0;
};

} // namespace shinybear

#endif