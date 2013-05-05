#ifndef SHINYBEAR_SCENEVIEW_H
#define SHINYBEAR_SCENEVIEW_H

#include "scene\LightNode.h"
#include "scene\MeshNode.h"
#include "scene\SceneManager.h"
#include "graphics\RenderTarget.h"
#include "graphics\VertexDeclaration.h"

#include <vector>
#include <d3dx9.h>
#include <d3d9.h>


namespace shinybear 
{ 
  class GraphicsProvider; class Shader;
  class LightVolume;
}

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
  void CreateVertexDecl();
  void CreateScreenVBuffer();

  // Renders the n/m pass.
  void RenderNormalPass();

  // Accumulates all lighting data into a buffer.
  void RenderLightPass();

  // Used to render a single light.
  void RenderLight(const Light *pLight);

  Mat4x4 CalcLightMatrix(const Light *pLight);

  // Renders the combined scene with the lighting data and geometry.
  void RenderCombinedScene();

  void DisplayRenderTarget(const RenderTarget *pTarget);

  static const int kScreenVertCount = 4;
  struct ScreenVertexData
  {
    Vector4 position;
    Vector2 texcoords;
  };
  static const ScreenVertexData kScreenVertices[kScreenVertCount];

  GraphicsProvider *m_pProvider;
  SceneManager *m_pScene;

  std::vector<const RenderData*> m_lights;
  std::vector<const RenderData*> m_meshes;
  std::vector<const RenderData*> m_alphaList;

  bool m_isRendering;
  
  Shader *m_pLightShader;
  Shader *m_pGBufferShader;
  Shader *m_pCombineShader;

  LightVolume *m_pQuadVolume;
  LightVolume *m_pSphereVolume;
  LightVolume *m_pConeVolume;

  IDirect3DVertexBuffer9 *m_pScreenVBuffer;

  RenderTarget *m_pNormalTarget;
  RenderTarget *m_pDiffuseTarget;
  RenderTarget *m_pSpecularTarget;
  RenderTarget *m_pDepthTarget;

  VertexDeclaration m_voxelDeclaration;
  VertexDeclaration m_textureDeclaration;
  VertexDeclaration m_lightDeclaration;

};

} // namespace shinybear

#endif // SHINYBEAR_SCENEVIEW_H