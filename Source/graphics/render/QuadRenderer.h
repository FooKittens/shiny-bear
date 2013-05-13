#ifndef SHINYBEAR_QUADRENDERER_H
#define SHINYBEAR_QUADRENDERER_H

#include "util\math\Math.h"
#include <d3d9.h>

namespace shinybear { class GraphicsProvider; class VertexDeclaration; }

namespace shinybear
{
// Used to render a fullscreen quad to help with deferred shading.
class QuadRenderer
{
public:
  QuadRenderer(GraphicsProvider *pProvider);
  ~QuadRenderer();

  void Initialize();

  void Render(const Vector2 &min, const Vector2 &max);

private:
  static const DWORD kQuadFVF;
  GraphicsProvider *m_pProvider;
  VertexDeclaration *m_pDecl;
};

} // namespace shinybear

#endif // SHINYBEAR_QUADRENDERER_H