#ifndef SHINYBEAR_SCENEMANAGER_H
#define SHINYBEAR_SCENEMANAGER_H

#include "graphics\Light.h"
#include <d3dx9.h>
#include <vector>

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

  void AddLight(Light *pLight);
  void RemoveLight(Light *pLight);

  void PushMatrix(const D3DXMATRIX &mat);
  void PopMatrix();

  const D3DXMATRIX * const GetTransform() const { return m_pMatStack->GetTop(); }
  SceneNode *GetRoot() const { return m_pRoot; }
  UINT GetLightCount() const { return m_lights.size(); }

  // used for iteration.
  Light *GetLight(UINT index) const { return m_lights[index]; }

  void OnDeviceLost();
  void OnDeviceReset();

private:
  GraphicsProvider *m_pGraphicsProvider;
  SceneNode *m_pRoot;
  ID3DXMatrixStack *m_pMatStack;
  std::vector<Light *> m_lights;
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