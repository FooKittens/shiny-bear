#ifndef SHINYBEAR_SCENEVIEW_H
#define SHINYBEAR_SCENEVIEW_H

#include "scene\LightNode.h"
#include "scene\MeshNode.h"
#include <vector>

namespace shinybear { class SceneManager; class GraphicsProvider; }

struct ID3DXEffect;

namespace shinybear
{

class SceneView
{
public:
  SceneView(GraphicsProvider *pProvider, SceneManager *pScene);
  ~SceneView();

  void Render();

  void AddMeshNode(MeshNode *pMesh);
  void AddLight(LightNode *pLight);

  void OnDeviceLost();
  void OnDeviceReset();

private:

  GraphicsProvider *m_pProvider;
  SceneManager *m_pScene;

  std::vector<LightNode*> m_lights;
  std::vector<MeshNode*> m_meshes;
  bool m_isRendering;
  ID3DXEffect *m_pShader;
  wchar_t *m_pShaderPath;
  IDirect3DVertexDeclaration9 *m_pDecl;
};

} // namespace shinybear

#endif // SHINYBEAR_SCENEVIEW_H