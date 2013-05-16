#include "graphics\effects\SSAOShader.h"
#include "graphics\Shader.h"
#include "graphics\RenderTarget.h"
#include "graphics\Texture.h"
#include "graphics\render\QuadRenderer.h"
#include "base\system\GraphicsProvider.h"
#include "resource\ResourceManager.h"
#include "scene\camera\Camera.h"
#include "util\SBUtil.h"

namespace shinybear
{

SSAOShader::SSAOShader(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
  m_pShader = nullptr;
}

SSAOShader::~SSAOShader()
{
  ResourceManager::DeleteResource(m_pShader);
  delete m_pShader;
  m_pShader = nullptr;
  delete m_pQuadRenderer;

  ResourceManager::DeleteResource(m_pAOTarget);
  delete m_pAOTarget;

  ResourceManager::DeleteResource(m_pNoiseTexture);
  delete m_pNoiseTexture;
  
}

void SSAOShader::Initialize()
{
  m_pShader = DBG_NEW Shader(m_pProvider);
  m_pShader->LoadFromFile("res\\shaders\\SSAOShader.fx");
  ResourceManager::RegisterResource(m_pShader, "SSAOShader");

  m_pNoiseTexture = Texture::FromFile(m_pProvider, "res\\images\\normal4x.png");
  ResourceManager::RegisterResource(m_pNoiseTexture, "SSAO_NOISETEXTURE");

  m_pQuadRenderer = DBG_NEW QuadRenderer(m_pProvider);
  m_pQuadRenderer->Initialize();

  m_pAOTarget = DBG_NEW RenderTarget(m_pProvider, D3DFMT_A16B16G16R16F);
  m_pAOTarget->OnDeviceReset(m_pProvider);
}

void SSAOShader::RenderOcclusionMap(RenderTarget *pDepthTarget,
  RenderTarget *pNormalTarget, Camera *pCamera)
{
  // This is needed for mapping pixels to texels in directx9.
  Vector2 halfPixel(
    0.5f / (float)m_pProvider->GetDisplayMode().width,
    0.5f / (float)m_pProvider->GetDisplayMode().height
  );

  Vector2 dimensions(
    (float)m_pProvider->GetDisplayMode().width,
    (float)m_pProvider->GetDisplayMode().height
  );

  m_pProvider->GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
  m_pProvider->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

  m_pShader->SetActiveTechnique("SSAOTech");
  m_pAOTarget->Activate(0);

  m_pProvider->GetDevice()->Clear(0, 0, D3DCLEAR_TARGET, 0xFFFFFFFF, 0.0f, 0);
  
  m_pShader->SetTexture("g_noiseMap", m_pNoiseTexture->GetTexture());
  m_pShader->SetTexture("g_depthMap", pDepthTarget->GetTexture());
  m_pShader->SetTexture("g_normalMap", pNormalTarget->GetTexture());
  m_pShader->SetVector2("g_halfPixel", halfPixel);
  m_pShader->SetVector2("g_dimensions", dimensions);
  m_pShader->SetMatrix("g_projection", pCamera->GetProjectionMatrix());
  m_pShader->SetMatrix("g_invProjection", pCamera->GetProjectionMatrix().Inverse());
  m_pShader->SetFloat("g_zFar", pCamera->GetViewDistance());

  m_pShader->Begin();
  m_pShader->BeginPass(0);
  m_pQuadRenderer->Render(Vector2::kOne * -1.0f, Vector2::kOne);

  m_pShader->EndPass();
  m_pShader->End();

  m_pAOTarget->Deactivate();

  
  m_pProvider->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
  m_pProvider->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

} // namespace shinybear