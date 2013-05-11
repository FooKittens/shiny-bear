#include "graphics\Renderer.h"
#include "graphics\RenderTarget.h"
#include "graphics\render\IDrawable.h"
#include "resource\ResourceManager.h"
#include "util\SBUtil.h"

#include <cassert>

namespace shinybear
{

Renderer::Renderer(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;

  // RenderTargets.
  m_pNormalTarget   = nullptr;
  m_pLightTarget    = nullptr;
  m_pDepthTarget    = nullptr;
  m_pMaterialTarget = nullptr;

  // Shaders.
  m_pGBufferShader  = nullptr;
  m_pLightShader    = nullptr;
  m_pCombineShader  = nullptr;
  
  m_pCamera = nullptr;
  m_isInitialized = false;
}

Renderer::~Renderer()
{
  m_renderList.clear();
  m_geometryList.clear();
  m_lights.clear();

  ResourceManager::DeleteResource(m_pNormalTarget);
  ResourceManager::DeleteResource(m_pLightTarget);
  ResourceManager::DeleteResource(m_pMaterialTarget);
  ResourceManager::DeleteResource(m_pDepthTarget);
}

void Renderer::Initialize()
{
  m_pNormalTarget   = DBG_NEW RenderTarget(m_pProvider, D3DFMT_A16B16G16R16F);
  m_pLightTarget    = DBG_NEW RenderTarget(m_pProvider, D3DFMT_A16B16G16R16F);
  m_pMaterialTarget = DBG_NEW RenderTarget(m_pProvider, D3DFMT_A16B16G16R16F);
  m_pDepthTarget    = DBG_NEW RenderTarget(m_pProvider, D3DFMT_R32F);
  
  // Register resources for automatic updating.
  ResourceManager::RegisterResource(m_pNormalTarget, "RendererNormalTarget");
  ResourceManager::RegisterResource(m_pLightTarget, "RendererLightTarget");
  ResourceManager::RegisterResource(m_pMaterialTarget, "RendererMaterialTarget");
  ResourceManager::RegisterResource(m_pDepthTarget, "RendererDepthTarget");

  // Retrieve shader resources from content pipeline.
  m_pGBufferShader = ResourceManager::GetResource<Shader>("DeferredGBufferShader");
  m_pLightShader   = ResourceManager::GetResource<Shader>("DeferredLightBufferShader");
  m_pCombineShader = ResourceManager::GetResource<Shader>("DeferredCombineShader");
}

void Renderer::RenderScene()
{
  // Make sure the renderer is initialized.
  assert(m_isInitialized && "Renderer not initialized!");

  // Updates the render list with visible geometry.
  UpdateRenderList();

  // Clear render targets to correct colors.
  ClearGBuffer();
  
}

void Renderer::UpdateRenderList()
{
  auto it = m_geometryList.begin();
  auto end = m_geometryList.end();
  while(it != end)
  {
    m_renderList.push_front(*it++);
  }
}

void Renderer::OnDeviceReset(GraphicsProvider *pProvider)
{

}

void Renderer::OnDeviceLost()
{

}

void Renderer::AddDrawable(IDrawable *pItem)
{
  m_geometryList.push_front(pItem);
}

void Renderer::RemoveDrawable(const IDrawable *pItem)
{
  auto it = m_geometryList.cbegin();
  auto end = m_geometryList.cend();
  
  while(it != end)
  {
    if(*it == pItem)
    {
      m_geometryList.erase(it);
      break;
    }
    ++it;
  }
}

void Renderer::AddLight(BaseLight *pLight)
{
  m_lights.push_front(pLight);
}

void Renderer::RemoveLight(const BaseLight *pLight)
{
  auto it = m_lights.cbegin();
  auto end = m_lights.cend();
  
  while(it != end)
  {
    if(*it == pLight)
    {
      m_lights.erase(it);
      break;
    }
    ++it;
  }
}

void Renderer::SetCamera(Camera *pCamera)
{
  m_pCamera = pCamera;
}

const Camera * const Renderer::GetCamera() const
{
  return m_pCamera;
}

} // namespace shinybear
