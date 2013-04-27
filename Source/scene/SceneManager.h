#ifndef SHINYBEAR_SCENEMANAGER_H
#define SHINYBEAR_SCENEMANAGER_H

#include <d3dx9.h>

namespace shinybear
{
  class GraphicsProvider; class SceneNode;
}

namespace shinybear
{

class SceneManager
{
public:
  SceneManager(GraphicsProvider *pProvider);
  ~SceneManager();

  void Update(double elapsedSeconds);
  void Render();

  void PushMatrix(const D3DXMATRIX &mat);
  void PopMatrix();

private:
  GraphicsProvider *m_pGraphicsProvider;
  SceneNode *m_pRoot;
  ID3DXMatrixStack *m_pMatStack;
};


inline void SceneManager::PushMatrix(const D3DXMATRIX &mat)
{
  m_pMatStack->Push();
  m_pMatStack->MultMatrixLocal(&mat);
}

inline void SceneManager::PopMatrix()
{
  m_pMatStack->Pop();
}

} // namespace shinybear

#endif