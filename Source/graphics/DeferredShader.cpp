#include "graphics\DeferredShader.h"
#include "graphics\Renderer.h"
#include "graphics\RenderTarget.h"
#include "graphics\Shader.h"
#include "graphics\VertexManager.h"
#include "graphics\VertexDeclaration.h"
#include "graphics\render\QuadRenderer.h"
#include "graphics\render\IDrawable.h"
#include "graphics\lighting\Lighting.h"
#include "graphics\LightVolume.h"
#include "scene\camera\Camera.h"
#include "base\system\GraphicsProvider.h"
#include "resource\ResourceManager.h"


#include "util\SBUtil.h"

namespace shinybear
{

DeferredShader::DeferredShader(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
  m_currentPass = P_NONE;
  m_pCamera = nullptr;
}

DeferredShader::~DeferredShader()
{
  delete m_pQuadRenderer;
}

void DeferredShader::Initialize()
{
  // Create render targets.
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
  m_pLightShader   = ResourceManager::GetResource<Shader>("DeferredLightShader");
  m_pCombineShader = ResourceManager::GetResource<Shader>("DeferredCombineShader");

  // Create a utility object for rendering fullscreen quads.
  m_pQuadRenderer = DBG_NEW QuadRenderer(m_pProvider);
  m_pQuadRenderer->Initialize();

  // Retrieve vertex declaration in use when rendering g-buffer.
  m_pGeometryDecl = VertexManager::GetVertexDeclaration(VertexDecl::VD_POSITION_COLOR_NORMAL);

  // Retrieve vertex declaration in use when rendering lights.
  m_pLightDecl = VertexManager::GetVertexDeclaration(VertexDecl::VD_POSITION);

  // Create a light volume for point lights.
  m_pSphereVolume = DBG_NEW LightVolume(m_pProvider);
  m_pSphereVolume->MakeSphere();
  ResourceManager::RegisterResource(m_pSphereVolume, "SphereLightVolume");
}

void DeferredShader::ClearGBuffer()
{
  // Disable z-writing or everything will be behind the render targets.
  m_pProvider->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

  m_pGBufferShader->SetActiveTechnique("ClearGBufferTech");
  
  m_pGBufferShader->Begin();
  m_pGBufferShader->BeginPass(0);

  // Render a full screen quad to clear the GBuffer.
  m_pQuadRenderer->Render(-Vector2::kOne, Vector2::kOne);

  m_pGBufferShader->EndPass();
  m_pGBufferShader->End();

  // Reset z-writing to its previous state.
  m_pProvider->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DeferredShader::BeginGBuffer()
{
  m_currentPass = P_GBUFFER;
  
  // Set normals and specular in render target 0.
  m_pNormalTarget->Activate(0);
  
  // Set depth as rendertarget 1.
  m_pDepthTarget->Activate(1);

  // Set material buffer in target 2.
  m_pMaterialTarget->Activate(2);
  
  // Clear the GBuffer to correct colors for each buffer type.
  ClearGBuffer();

  // Activate vertex declaration for geometry stage.
  m_pGeometryDecl->Activate();
  
  // Set gbuffer technique.
  m_pGBufferShader->SetActiveTechnique("NormalTech");

  // Update camera variables in shader.
  m_pGBufferShader->SetFloat("g_zfar", m_pCamera->GetViewDistance());
  m_pGBufferShader->SetMatrix("g_view", m_pCamera->GetViewMatrix());

  m_pGBufferShader->Begin();
}

void DeferredShader::RenderGeometry(IDrawable *pDrawable)
{
  assert(m_currentPass == P_GBUFFER && "Can only render geometry in gbuffer!");
  Mat4x4 wvIT = pDrawable->GetWorldMatrix();
  wvIT *= m_pCamera->GetViewMatrix();

  m_pGBufferShader->SetMatrix("g_world", pDrawable->GetWorldMatrix());  
  m_pGBufferShader->SetMatrix("g_inverseTranspose", wvIT.Inverse().Transpose());
  m_pGBufferShader->SetMatrix("g_WVP", wvIT * m_pCamera->GetProjectionMatrix());

  m_pGBufferShader->BeginPass(0);
  pDrawable->Render(m_pProvider);
  m_pGBufferShader->EndPass();
}

void DeferredShader::EndGBuffer()
{
  m_pGBufferShader->End();
  
  // Deactivate render targets.
  m_pNormalTarget->Deactivate();
  m_pDepthTarget->Deactivate();
  m_pMaterialTarget->Deactivate();

  m_currentPass = P_NONE;
}

void DeferredShader::BeginLightPass()
{
  m_currentPass = P_LIGHTING;
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  // We don't need z tests for lights.
  pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

  // Activate light accumulation target.
  m_pLightTarget->Activate(0);

  // Clear light target.
  pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);


  // This is needed for mapping pixels to texels in directx9.
  Vector2 halfPixel(
    0.5f / (float)m_pProvider->GetDisplayMode().width,
    0.5f / (float)m_pProvider->GetDisplayMode().height
  );

  m_pLightDecl->Activate();

  m_pLightShader->SetActiveTechnique("LightTech");

  m_pLightShader->SetVector2("g_halfPixel", halfPixel);
  m_pLightShader->SetMatrix("g_invProjection", m_pCamera->GetProjectionMatrix().Inverse());
  m_pLightShader->SetTexture("g_normalMap", m_pNormalTarget->GetTexture());
  m_pLightShader->SetTexture("g_depthMap", m_pDepthTarget->GetTexture());
  m_pLightShader->SetMatrix("g_invView", m_pCamera->GetViewMatrix().Inverse().Transpose());
  m_pLightShader->SetMatrix("g_view", m_pCamera->GetViewMatrix());

  m_pLightShader->Begin();
}

void DeferredShader::RenderLight(BaseLight *pLight)
{
  // Set lighttype in shader.
  int lightType = static_cast<int>(pLight->GetType());
  m_pLightShader->SetRaw("g_lightType", (void*)(&lightType), sizeof(int));

  // Set light color, same for all lights.
  m_pLightShader->SetVector4("g_lightColor", pLight->GetColor());

  switch(pLight->GetType())
  {
  case LightType::LT_AMBIENT:
    RenderAmbientLight(pLight);
    break;
  case LightType::LT_DIRECTIONAL:
    RenderDirectionalLight(dynamic_cast<DirectionalLight*>(pLight));
    break;
  case LightType::LT_POINT:
    RenderPointLight(dynamic_cast<PointLight*>(pLight));
    break;
  case LightType::LT_SPOT:
    assert(false && "Spotlights are not implemented!");
    break;
  }
}

void DeferredShader::RenderAmbientLight(BaseLight *pLight)
{
  m_pLightShader->SetMatrix("g_WVP", Mat4x4::kIdentity);

  m_pLightShader->BeginPass(0);
  m_pQuadRenderer->Render(Vector2::kOne * -1.0f, Vector2::kOne);
  m_pLightShader->EndPass();
}

void DeferredShader::RenderDirectionalLight(DirectionalLight *pLight)
{
  m_pLightShader->SetMatrix("g_WVP", Mat4x4::kIdentity);
  m_pLightShader->SetVector3("g_lightDir", pLight->m_direction);

  m_pLightShader->BeginPass(0);
  m_pQuadRenderer->Render(Vector2::kOne * -1.0f, Vector2::kOne);
  m_pLightShader->EndPass();
}

void DeferredShader::RenderPointLight(PointLight *pLight)
{
  Mat4x4 wvp = Mat4x4::CreateScale(pLight->m_range) *
    Mat4x4::CreateTranslation(pLight->m_position) *
    m_pCamera->GetViewMatrix() *
    m_pCamera->GetProjectionMatrix();

  if(m_pCamera->GetViewMatrix().Transform(pLight->m_position).Length() < pLight->m_range / 2.0f)
  {
    m_pProvider->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  }

  m_pLightShader->SetMatrix("g_WVP", wvp);
  m_pLightShader->SetVector3("g_lightWP", pLight->m_position);
  m_pLightShader->SetFloat("g_att0", pLight->att0);
  m_pLightShader->SetFloat("g_att1", pLight->att1);
  m_pLightShader->SetFloat("g_att2", pLight->att2);

  m_pLightShader->BeginPass(0);
  m_pSphereVolume->Render();
  //m_pQuadRenderer->Render(Vector2::kOne * -1.0f, Vector2::kOne);
  m_pLightShader->EndPass();
  m_pProvider->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void DeferredShader::EndLightPass()
{
  m_currentPass = P_NONE;
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();
  m_pLightShader->End();
  m_pLightTarget->Deactivate();
  
  pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DeferredShader::RenderCompositeImage()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();
  pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

  m_pCombineShader->SetTexture("g_lightMap", m_pLightTarget->GetTexture());
  m_pCombineShader->SetTexture("g_materialMap", m_pMaterialTarget->GetTexture());

  m_pCombineShader->SetActiveTechnique("CombineTech");
  m_pCombineShader->Begin();
  m_pCombineShader->BeginPass(0);
  m_pQuadRenderer->Render(-Vector2::kOne, Vector2::kOne);
  m_pCombineShader->EndPass();
  m_pCombineShader->End();

  pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DeferredShader::DisplayRenderTarget(RenderTarget *pTarget)
{
  m_pCombineShader->SetActiveTechnique("RenderToScreen");
  
  m_pCombineShader->SetTexture("g_texture", pTarget->GetTexture());

  m_pCombineShader->Begin();
  m_pCombineShader->BeginPass(0);

  // Render a full-screen quad to clear the GBuffer.
  m_pQuadRenderer->Render(-Vector2::kOne, Vector2::kOne);

  m_pCombineShader->EndPass();
  m_pCombineShader->End();
}

void DeferredShader::OnDeviceLost()
{

}

void DeferredShader::OnDeviceReset(GraphicsProvider *pProvider)
{

}

} // namespace shinybear