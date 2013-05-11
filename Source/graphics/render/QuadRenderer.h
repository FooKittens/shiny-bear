#ifndef SHINYBEAR_QUADRENDERER_H
#define SHINYBEAR_QUADRENDERER_H

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{
// Used to render a fullscreen quad to help with deferred shading.
class QuadRenderer
{
public:
  QuadRenderer(GraphicsProvider *pProvider);
  ~QuadRenderer();

  void Initialize();

  void Render();
private:
  GraphicsProvider *m_pProvider;
};

} // namespace shinybear

#endif // SHINYBEAR_QUADRENDERER_H