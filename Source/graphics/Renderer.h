#ifndef SHINYBEAR_RENDERER_H
#define SHINYBEAR_RENDERER_H

#include "resource\types\IGraphicsResource.h"

#include <list>

// Forward declarations.
namespace shinybear
{
  class GraphicsProvider; class Camera;
  class IDrawable; class BaseLight;
  class RenderTarget; class Shader;
  class QuadRenderer; class VertexDeclaration;
  class DeferredShader;
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
protected:

private:
  // Performs culling of the scene and pushes geometry to the render list.
  void UpdateRenderList();

  // Renders materials, depth, normals etc into render targets.
  void RenderGBuffer();

  void RenderLightBuffer();
  
  // Used to access the graphics device.
  GraphicsProvider *m_pProvider;

  // Holds the visible geometry after culling.
  // Will be rendered using deferred shading.
  std::list<IDrawable*> m_renderList;

  // List of visible geometry requiring the alpha pass.
  std::list<IDrawable*> m_alphaList;

  // List of visible geometry that should be rendered using forward rendering.
  std::list<IDrawable*> m_forwardList;

  // Holds all geometry registered for drawing.
  std::list<IDrawable*> m_geometryList;

  // Holds all the lights in the scene.
  std::list<BaseLight*> m_lights;

  // Camera currently used to render the scene.
  Camera *m_pCamera;

  DeferredShader *m_pDShader;

  QuadRenderer *m_pQuadRenderer;

  bool m_isInitialized;

  VertexDeclaration *m_pMeshDecl;
};

} // namespace shinybear

#endif // SHINYBEAR_RENDERER_H