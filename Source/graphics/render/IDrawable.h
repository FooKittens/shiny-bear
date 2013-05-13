#ifndef SHINYBEAR_IDRAWABLE_H
#define SHINYBEAR_IDRAWABLE_H

#include "util\math\Math.h"
#include "graphics\Renderer.h"

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{
class IDrawable
{
public:
  virtual const Mat4x4 &GetWorldMatrix() const = 0;
  virtual float GetBoundingRadius() const = 0;
  virtual void Render(GraphicsProvider *pProvider) = 0;
  virtual RenderPass::E GetRenderPass() const = 0;
};

} // namespace shinybear

#endif