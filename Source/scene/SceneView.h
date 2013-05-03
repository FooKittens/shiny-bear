#ifndef SHINYBEAR_SCENEVIEW_H
#define SHINYBEAR_SCENEVIEW_H

#include "scene\LightNode.h"
#include "scene\MeshNode.h"
#include "scene\SceneManager.h"
#include "graphics\RenderTarget.h"

#include <vector>
#include <d3dx9.h>
#include <d3d9.h>


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
  void CreateVertexDecl();
  void LoadShader();
  void CreateScreenVBuffer();

  // Renders geoemtry with a specified technique.
  void RenderGeometry(D3DXHANDLE hTech);

  // Renders the n/m pass.
  void RenderNormalPass();

  // Accumulates all lighting data into a buffer.
  void RenderLightPass();

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
  ID3DXEffect *m_pShader;
  wchar_t *m_pShaderPath;
  IDirect3DVertexDeclaration9 *m_pDecl;
  IDirect3DVertexDeclaration9 *m_pTextureDecl;

  IDirect3DVertexBuffer9 *m_pScreenVBuffer;

  // Handles to matrices on the shader.
  D3DXHANDLE m_hFxWorld;
  D3DXHANDLE m_hFxView;
  D3DXHANDLE m_hFxProjection;
  D3DXHANDLE m_hFxTexture;
  D3DXHANDLE m_hFxLight;
  D3DXHANDLE m_hFxCamPos;
  D3DXHANDLE m_hFxNormalMap;
  D3DXHANDLE m_hFxFarPlane;
  D3DXHANDLE m_hFxNearPlane;
  D3DXHANDLE m_hFxDiffuseMap;
  D3DXHANDLE m_hFxSpecularMap;

  D3DXHANDLE m_hFxNormalTech;
  D3DXHANDLE m_hFxGeometryTech;
  D3DXHANDLE m_hFxDiffuseTech;
  D3DXHANDLE m_hFxSpecularTech;
  D3DXHANDLE m_hFxCombineTech;
  D3DXHANDLE m_hFxRenderScreenTech;
  D3DXHANDLE m_hFxLightMRTTech;

  RenderTarget *m_pNormalTarget;
  RenderTarget *m_pDiffuseTarget;
  RenderTarget *m_pSpecularTarget;
  RenderTarget *m_pDepthTarget;
};

} // namespace shinybear

#endif // SHINYBEAR_SCENEVIEW_H