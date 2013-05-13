#ifndef SHINYBEAR_DEFERREDSHADER_H
#define SHINYBEAR_DEFERREDSHADER_H

#include "resource\types\IGraphicsResource.h"
#include "util\math\Math.h"

namespace shinybear
{ 
  class GraphicsProvider; class RenderTarget;
  class Shader; class QuadRenderer;
  class Camera; class IDrawable;
  class BaseLight; class VertexDeclaration;
  class DirectionalLight; class PointLight;
  class SpotLight; class LightVolume;
}

namespace shinybear
{

class DeferredShader : public IGraphicsResource
{
public:
  DeferredShader(GraphicsProvider *pProvider);
  ~DeferredShader();

  void Initialize();
  
  // Readies the gbuffer, takes a parameter in for settings up shader.
  void BeginGBuffer();

  // Renders a piece of geometry to the gbuffer.
  void RenderGeometry(IDrawable *pDrawable);

  // Finalizes the gbuffer.
  void EndGBuffer();

  // Begins the light accumulation pass.
  void BeginLightPass();

  // Renders a light to the light buffer.
  void RenderLight(BaseLight *pLight);

  // Stop light pass.
  void EndLightPass();

  // Renders the composite image of the gbuffer and light buffer.
  void RenderCompositeImage();

  void SetCamera(Camera *pCamera);

  void OnDeviceLost();
  void OnDeviceReset(GraphicsProvider *pProvider);

  RenderTarget * const GetNormalTarget() const;
  RenderTarget * const GetLightTarget() const;
  RenderTarget * const GetDepthTarget() const;
  RenderTarget * const GetMaterialTarget() const;

  // used for debugging.
  void DisplayRenderTarget(RenderTarget *pTarget);

private:
  enum Passes
  {
    P_GBUFFER = 1,
    P_LIGHTING = 2,
    P_COMBINE = 3,
    P_NONE = 4
  } m_currentPass;

  // Clears the GBuffer.
  void ClearGBuffer();

  void RenderAmbientLight(BaseLight *pLight);
  void RenderDirectionalLight(DirectionalLight *pLight);
  void RenderPointLight(PointLight *pLight);
  //void RenderSpotLight(SpotLight *pLight);

  GraphicsProvider *m_pProvider;

  RenderTarget *m_pMaterialTarget;
  RenderTarget *m_pNormalTarget;
  RenderTarget *m_pLightTarget;
  RenderTarget *m_pDepthTarget;

  Shader *m_pGBufferShader;
  Shader *m_pLightShader;
  Shader *m_pCombineShader;

  LightVolume *m_pSphereVolume;

  QuadRenderer *m_pQuadRenderer;

  VertexDeclaration *m_pGeometryDecl;
  VertexDeclaration *m_pLightDecl;

  Camera *m_pCamera;
};

inline RenderTarget * const DeferredShader::GetNormalTarget() const
{
  return m_pNormalTarget;
}

inline RenderTarget * const DeferredShader::GetMaterialTarget() const
{
  return m_pMaterialTarget;
}

inline RenderTarget * const DeferredShader::GetLightTarget() const
{
  return m_pLightTarget;
}

inline RenderTarget * const DeferredShader::GetDepthTarget() const
{
  return m_pDepthTarget;
}

inline void DeferredShader::SetCamera(Camera *pCamera)
{
  m_pCamera = pCamera;
}

} // namespace shinybear
#endif
