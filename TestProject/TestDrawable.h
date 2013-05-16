#ifndef SHINYBEAR_TESTDRAWABLE_H
#define SHINYBEAR_TESTDRAWABLE_H

#include <graphics\render\IDrawable.h>

namespace shinybear { class Mesh; }

class TestDrawable : public shinybear::IDrawable
{
public:
  TestDrawable();
  ~TestDrawable();

  const shinybear::Mat4x4 &GetWorldMatrix() const { return m_world; }
  float GetBoundingRadius() const { return 5.0f; }
  void Render(shinybear::GraphicsProvider *pProvider);
  shinybear::RenderPass::E GetRenderPass() const { return shinybear::RenderPass::DEFERRED; }

  void SetMesh(shinybear::Mesh *pMesh) {  m_pMesh = pMesh; }

   shinybear::Mat4x4 m_world;
private:

 
  shinybear::Mesh *m_pMesh;
};



#endif // SHINYBEAR_TESTDRAWABLE_H