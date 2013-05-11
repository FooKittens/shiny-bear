#ifndef SHINYBEAR_RENDERER_H
#define SHINYBEAR_RENDERER_H

#include "resource\types\IGraphicsResource.h"

#include <list>

namespace shinybear
{
  class GraphicsProvider; class Camera;
  class IDrawable; class BaseLight;
  class RenderTarget; class Shader;
}

namespace shinybear
{

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

  // Clears the GBuffer.
  void ClearGBuffer();
  
  // Used to access the graphics device.
  GraphicsProvider *m_pProvider;

  // Holds the visible geometry after culling.
  std::list<IDrawable*> m_renderList;

  // Holds all geometry registered for drawing.
  std::list<IDrawable*> m_geometryList;

  // Holds all the lights in the scene.
  std::list<BaseLight*> m_lights;

  // Camera currently used to render the scene.
  Camera *m_pCamera;

  RenderTarget *m_pNormalTarget;
  RenderTarget *m_pLightTarget;
  RenderTarget *m_pDepthTarget;
  RenderTarget *m_pMaterialTarget;

  Shader *m_pGBufferShader;
  Shader *m_pLightShader;
  Shader *m_pCombineShader;

  bool m_isInitialized;
};

} // namespace shinybear

#endif // SHINYBEAR_RENDERER_H