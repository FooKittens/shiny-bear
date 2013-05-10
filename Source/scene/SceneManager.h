#ifndef SHINYBEAR_SCENEMANAGER_H
#define SHINYBEAR_SCENEMANAGER_H

#include "scene\Camera\Camera.h"
#include "graphics\Light.h"
#include "util\math\Math.h"
#include <d3dx9.h>
#include <vector>

namespace shinybear
{
  class GraphicsProvider; class SceneNode;
  class SceneView; class Mesh;
}

namespace shinybear
{

namespace RenderType
{
enum Enum
{
  MESH = 1,
  LIGHT = 2,
  ALPHA = 3,
};
}

struct RenderData
{
  RenderType::Enum type;
  SceneNode *pNode;
  Mat4x4 world;
  union RD
  {
    Mesh *pMesh;
    Light *pLight;
  } data;
};

typedef std::vector<RenderData> RenderList;

class SceneManager
{
public:
  SceneManager(GraphicsProvider *pProvider);
  ~SceneManager();

  void Update(double elapsedSeconds);
  void Render();

  void PushMatrix(const D3DXMATRIX &mat);
  void PopMatrix();

  const D3DXMATRIX * const GetTransform() const { return m_pMatStack->GetTop(); }
  SceneNode *GetRoot() const { return m_pRoot; }

  void OnDeviceLost();
  void OnDeviceReset();

  void SetCamera(Camera *pCamera);
  Camera *GetCamera() const;

  void PushRenderData(const RenderData &data);

private:
  GraphicsProvider *m_pGraphicsProvider;
  SceneNode *m_pRoot;
  ID3DXMatrixStack *m_pMatStack;
  SceneView *m_pView;
  RenderList m_renderList;
  Camera *m_pCamera;
  bool m_isCameraSet;
};

inline void SceneManager::PushRenderData(const RenderData &data)
{
  m_renderList.push_back(data);
}

inline void SceneManager::PushMatrix(const D3DXMATRIX &mat)
{
  m_pMatStack->Push();
  m_pMatStack->MultMatrixLocal(&mat);
}

inline void SceneManager::PopMatrix()
{
  m_pMatStack->Pop();
}

inline void SceneManager::SetCamera(Camera *pCamera)
{
  m_pCamera = pCamera;
}

inline Camera *SceneManager::GetCamera() const
{
  return m_pCamera;
}

} // namespace shinybear

#endif