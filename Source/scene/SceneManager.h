#ifndef SHINYBEAR_SCENEMANAGER_H
#define SHINYBEAR_SCENEMANAGER_H

#include "scene\CameraNode.h"
#include "graphics\Light.h"
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
  D3DXMATRIX world;
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

  void SetCamera(CameraNode *pCamera);
  CameraNode *GetCamera() const;

  void PushRenderData(const RenderData &data);

private:
  GraphicsProvider *m_pGraphicsProvider;
  SceneNode *m_pRoot;
  ID3DXMatrixStack *m_pMatStack;
  SceneView *m_pView;
  RenderList m_renderList;
  CameraNode *m_pCamera;
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

inline void SceneManager::SetCamera(CameraNode *pCamera)
{
  if(m_isCameraSet)
  {
    m_pRoot->Detach(m_pCamera);
  }
  m_pCamera = pCamera;
  m_pRoot->Attach(m_pCamera);
}

inline CameraNode *SceneManager::GetCamera() const
{
  return m_pCamera;
}

} // namespace shinybear

#endif