#include "graphics\Renderer.h"
#include "graphics\Shader.h"
#include "graphics\RenderTarget.h"
#include "graphics\VertexDeclaration.h"
#include "graphics\effects\DeferredShader.h"
#include "graphics\effects\SSAOShader.h"
#include "graphics\render\IDrawable.h"
#include "graphics\render\QuadRenderer.h"
#include "resource\ResourceManager.h"
#include "scene\camera\Camera.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"

#include <cassert>

namespace shinybear
{

Renderer::Renderer(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;

  m_pDShader = nullptr;
  m_pCombineShader = nullptr;

  m_pMeshDecl = nullptr;

  m_pCamera = nullptr;
  m_isInitialized = false;

  m_pDevice = pProvider->GetDevice();
}

Renderer::~Renderer()
{
  m_renderList.clear();
  m_geometryList.clear();
  m_lights.clear();

  ResourceManager::DeleteResource(m_pCombineShader);

  delete m_pDShader;
  delete m_pSSAOShader;
  delete m_pQuadRenderer;
  delete m_pCombineShader;
}

void Renderer::Initialize()
{
  m_pDShader = DBG_NEW DeferredShader(m_pProvider);
  m_pDShader->Initialize();

  m_pSSAOShader = DBG_NEW SSAOShader(m_pProvider);
  m_pSSAOShader->Initialize();

  m_pCombineShader = DBG_NEW Shader(m_pProvider);
  m_pCombineShader->LoadFromFile("res\\shaders\\CombineShader.fx");
  ResourceManager::RegisterResource(m_pCombineShader, "DS_COMBINESHADER");

  m_isInitialized = true;

  m_pQuadRenderer = DBG_NEW QuadRenderer(m_pProvider);
  m_pQuadRenderer->Initialize();
}

void Renderer::RenderScene()
{
  // Make sure the renderer is initialized.
  assert(m_isInitialized && "Renderer not initialized!");
  assert(m_pCamera && "Camera not set!");

  // Updates the render list with visible geometry.
  UpdateRenderList();
   
  // Render Geometry buffer.
  RenderGBuffer();

  // Render a light accumulation buffer.
  //RenderLightBuffer();

  // Merge render targets into a final result.
  //RenderComposite();

  m_pSSAOShader->RenderOcclusionMap(
    m_pDShader->GetDepthTarget(),
    m_pDShader->GetNormalTarget(),
    m_pCamera
  );

  //m_pCamera->RenderFrustum(m_pProvider);
  //DisplayRenderTarget(m_pDShader->GetLightTarget());
  DisplayRenderTarget(m_pSSAOShader->GetAOTarget());
  
}


void Renderer::RenderGBuffer()
{
  // No use rendering anything if we don't have a camera.
  if(!m_pCamera) return;

  m_pDShader->BeginGBuffer();

  auto it = m_renderList.begin();
  auto end = m_renderList.end();
  while(it != end)
  {
    m_pDShader->RenderGeometry(*it++);    
  }

  m_pDShader->EndGBuffer();
}

void Renderer::RenderLightBuffer()
{
  m_pDShader->BeginLightPass();

  auto it = m_lights.begin();
  auto end = m_lights.end();

  while(it != end)
  {
    m_pDShader->RenderLight(*it++);
  }

  m_pDShader->EndLightPass();
}

void Renderer::RenderComposite()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();
  pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    // This is needed for mapping pixels to texels in directx9.
  Vector2 halfPixel(
    0.5f / (float)m_pProvider->GetDisplayMode().width,
    0.5f / (float)m_pProvider->GetDisplayMode().height
  );

  m_pCombineShader->SetTexture("g_lightMap", m_pDShader->GetLightTarget()->GetTexture());
  m_pCombineShader->SetTexture("g_materialMap", m_pDShader->GetMaterialTarget()->GetTexture());
  m_pCombineShader->SetVector2("g_halfPixel", halfPixel);

  m_pCombineShader->SetActiveTechnique("CombineTech");
  m_pCombineShader->Begin();
  m_pCombineShader->BeginPass(0);
  m_pQuadRenderer->Render(-Vector2::kOne, Vector2::kOne);
  m_pCombineShader->EndPass();
  m_pCombineShader->End();

  pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void Renderer::DisplayRenderTarget(RenderTarget *pTarget)
{
  // This is needed for mapping pixels to texels in directx9.
  Vector2 halfPixel(
    0.5f / (float)m_pProvider->GetDisplayMode().width,
    0.5f / (float)m_pProvider->GetDisplayMode().height
  );

  m_pCombineShader->SetActiveTechnique("RenderToScreen");
  m_pCombineShader->SetTexture("g_texture", pTarget->GetTexture());
  m_pCombineShader->SetVector2("g_halfPixel", halfPixel);

  m_pCombineShader->Begin();
  m_pCombineShader->BeginPass(0);

  // Render a full-screen quad to clear the GBuffer.
  m_pQuadRenderer->Render(-Vector2::kOne, Vector2::kOne);

  m_pCombineShader->EndPass();
  m_pCombineShader->End();
}

// TODO : Perform culling etc.
void Renderer::UpdateRenderList()
{
  m_renderList.clear();
  m_alphaList.clear();
  m_forwardList.clear();

  auto it = m_geometryList.begin();
  auto end = m_geometryList.end();
  while(it != end)
  {
    // Sort geometry list into separate passes for alpha and forward rendering.
    switch((*it)->GetRenderPass())
    {
    case RenderPass::DEFERRED:
      m_renderList.push_front(*it++);
      break;
    case RenderPass::ALPHA:
      m_alphaList.push_front(*it++);
      break;
    case RenderPass::FORWARD:
      m_forwardList.push_front(*it++);
      break;
    default:
      assert(false && "Invalid Renderpass specified!");
    }
    
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
  m_pDShader->SetCamera(pCamera);
}

const Camera * const Renderer::GetCamera() const
{
  return m_pCamera;
}

} // namespace shinybear
