#ifndef SHINYBEAR_RENDERER_H
#define SHINYBEAR_RENDERER_H

#include "resource\types\IGraphicsResource.h"
#include "util\SBUtil.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <list>

// Forward declarations.
namespace shinybear
{
  class GraphicsProvider; class Camera;
  class IDrawable; class BaseLight;
  class RenderTarget; class Shader;
  class QuadRenderer; class VertexDeclaration;
  class DeferredShader; class SSAOShader;
}



namespace shinybear
{

// Enumeration for target render passes. Allows us to
// render transparent objects and debug geometry.
namespace RenderPass
{
  enum E
  {
    DEFERRED = 1,
    ALPHA = 2,
    FORWARD = 3,
  };
}

class Renderer : public IGraphicsResource
{
public:
  Renderer(GraphicsProvider *pProvider);
  ~Renderer();

  // Must be called before any rendering is done.
  void Initialize();

  // This is where the magic happens.
  void RenderScene();

  // For managing geometry with the renderer.
  void AddDrawable(IDrawable *pItem);
  void RemoveDrawable(const IDrawable *pItem);

  // Adds and removes lights in the scene.
  void AddLight(BaseLight *pLight);
  void RemoveLight(const BaseLight *pLight);

  void SetCamera(Camera *pCamera);
  const Camera * const GetCamera() const;

  // For IGraphicsResource interface.
  void OnDeviceReset(GraphicsProvider *pProvider);
  void OnDeviceLost();

  // Clears the backbuffer.
  void Clear(const D3DXCOLOR &color);
  void BeginScene();
  void EndScene();
  void Present();
protected:

private:
  void UpdateRenderList();

  // Renders materials, depth, normals etc into render targets.
  void RenderGBuffer();

  void RenderLightBuffer();

  // Render the composed image of all render targets and post-FX.
  void RenderComposite();
  
  // Used for debugging.
  void DisplayRenderTarget(RenderTarget *pTarget);

  // Used to access the graphics device.
  GraphicsProvider *m_pProvider;
  IDirect3DDevice9 *m_pDevice;

  // Holds all geometry begin drawn by the renderer.
  std::list<IDrawable*> m_geometryList;

  // Holds the visible geometry after culling.
  // Will be rendered using deferred shading.
  std::list<IDrawable*> m_renderList;
  
  // List of visible geometry requiring the alpha pass.
  std::list<IDrawable*> m_alphaList;

  // List of visible geometry that should be rendered using forward rendering.
  std::list<IDrawable*> m_forwardList;

  // Holds all the lights in the scene.
  std::list<BaseLight*> m_lights;

  // Camera currently used to render the scene.
  Camera *m_pCamera;

  // Shader used to render geometry data and light accumulation
  // to separate render targets.
  DeferredShader *m_pDShader;

  SSAOShader *m_pSSAOShader;

  // Shader used to combine the output of several render targets
  // to a final image for rendering.
  Shader *m_pCombineShader;

  QuadRenderer *m_pQuadRenderer;

  bool m_isInitialized;

  VertexDeclaration *m_pMeshDecl;
};

inline void Renderer::Clear(const D3DXCOLOR &col)
{
  m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
    col, 1.0f, 0);
}

inline void Renderer::BeginScene()
{
  HR(m_pDevice->BeginScene());
}

inline void Renderer::EndScene()
{
  HR(m_pDevice->EndScene());
}

inline void Renderer::Present()
{
  HR(m_pDevice->Present(0, 0, 0, 0));
}

} // namespace shinybear

#endif // SHINYBEAR_RENDERER_H