#ifndef SHINYBEAR_SCENEVIEW_H
#define SHINYBEAR_SCENEVIEW_H

#include "scene\LightNode.h"
#include "scene\MeshNode.h"
#include "scene\SceneManager.h"
#include <vector>

namespace shinybear { class GraphicsProvider; }

struct ID3DXEffect;

namespace shinybear
{

class SceneView
{
public:
  SceneView(GraphicsProvider *pProvider, SceneManager *pScene);
  ~SceneView();

  void Render(const RenderList &list);

  void OnDeviceLost();
  void OnDeviceReset();

private:

  GraphicsProvider *m_pProvider;
  SceneManager *m_pScene;

  std::vector<const RenderData*> m_lights;
  std::vector<const RenderData*> m_meshes;
  std::vector<const RenderData*> m_alphaList;

  bool m_isRendering;
  ID3DXEffect *m_pShader;
  wchar_t *m_pShaderPath;
  IDirect3DVertexDeclaration9 *m_pDecl;
};

} // namespace shinybear

#endif // SHINYBEAR_SCENEVIEW_H