#include "scene\SceneView.h"
#include "scene\SceneManager.h"
#include "scene\SceneNode.h"
#include "scene\CameraNode.h"
#include "base\system\GraphicsProvider.h"
#include "graphics\Mesh.h"
#include "graphics\Shader.h"
#include "graphics\LightVolume.h"
#include "util\SBUtil.h"
#include <cassert>
#include <d3dx9.h>

namespace shinybear
{

const SceneView::ScreenVertexData SceneView::kScreenVertices[kScreenVertCount] = 
{
  // Top right triangle
  { Vector4(-1, +1.0, 0, 1), Vector2(0, 0) },
  { Vector4(+1, +1.0, 0, 1), Vector2(1, 0) },
  { Vector4(-1, -1, 0, 1), Vector2(0, 1) },
  { Vector4(+1, -1, 0, 1), Vector2(1, 1) },
};

SceneView::SceneView(GraphicsProvider *pProvider, SceneManager *pScene)
  :m_voxelDeclaration(pProvider), m_textureDeclaration(pProvider),
  m_lightDeclaration(pProvider)
{
  m_pScene = pScene;
  m_pProvider = pProvider;
  m_isRendering = false;
  m_pScreenVBuffer = nullptr;

  wchar_t *pBuffer;
  GetAbsolutePath(L"res\\shaders\\LightShader.fx", &pBuffer);
  m_pLightShader = DBG_NEW Shader(pProvider);
  m_pLightShader->LoadFromFile(pBuffer);
  delete[] pBuffer;

  GetAbsolutePath(L"res\\shaders\\GBufferShader.fx", &pBuffer);
  m_pGBufferShader = DBG_NEW Shader(pProvider);
  m_pGBufferShader->LoadFromFile(pBuffer);
  delete[] pBuffer;

  GetAbsolutePath(L"res\\shaders\\CombineShader.fx", &pBuffer);
  m_pCombineShader = DBG_NEW Shader(pProvider);
  m_pCombineShader->LoadFromFile(pBuffer);
  delete[] pBuffer;

  m_pQuadVolume = DBG_NEW LightVolume(pProvider);
  m_pSphereVolume = DBG_NEW LightVolume(pProvider);
  m_pConeVolume = DBG_NEW LightVolume(pProvider);

  m_pQuadVolume->MakeQuad();
  m_pSphereVolume->MakeSphere();
  m_pConeVolume->MakeCone();

  m_pNormalTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A8R8G8B8);
  m_pDiffuseTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A8R8G8B8);
  m_pSpecularTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A8R8G8B8);
  m_pDepthTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_R32F);

  
  CreateVertexDecl();

  OnDeviceReset();
}

SceneView::~SceneView()
{
  delete m_pNormalTarget;
  delete m_pDiffuseTarget;
  delete m_pSpecularTarget;
  delete m_pDepthTarget;
  delete m_pLightShader;
  delete m_pGBufferShader;
  delete m_pCombineShader;
  delete m_pQuadVolume;
  delete m_pSphereVolume;
  delete m_pConeVolume;
  RELEASECOM(m_pScreenVBuffer);
}

void SceneView::Render(const RenderList &list)
{
  assert(!m_isRendering && "Sceneview already rendering!");
  m_isRendering = true;

  m_lights.clear();
  m_meshes.clear();  
  m_alphaList.clear();

  // Filter RenderData into different lists for different passes.
  for(UINT i = 0; i < list.size(); ++i)
  {
    if(list[i].type == RenderType::MESH)
    {
      m_meshes.push_back(&list[i]);
    } 
    else if(list[i].type == RenderType::LIGHT)
    {
      m_lights.push_back(&list[i]);
    }
    else if(list[i].type == RenderType::ALPHA)
    {
      m_alphaList.push_back(&list[i]);
    }
  }

  // Set matrices for geometry rendering.
  CameraNode *pCam = m_pScene->GetCamera();

  // TODO : FIX THE NEED FOR THIS!
  pCam->Update(0);

  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();  
  
  pDevice->EndScene();

  // Render N/M Buffer.
  RenderNormalPass();

  // Render lights into diffuse and specular buffers.
  RenderLightPass();

  // Blend the combined image of the NM and light buffer.
  RenderCombinedScene();

  //DisplayRenderTarget(m_pSpecularTarget);

  pDevice->BeginScene();

  m_isRendering = false;
}

void SceneView::RenderNormalPass()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();
  
  // Activate the render target for normals, in slot 0.
  m_pNormalTarget->Activate(0);
  m_pDepthTarget->Activate(1);
    
  // Normal voxel vertexdeclaration.
  m_voxelDeclaration.Activate();

  // Clear render target.
  HR(pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
    0, 1.0f, 0));

  // Begin rendering.
  HR(pDevice->BeginScene());

  m_pGBufferShader->SetActiveTechnique("NormalTech");
  
  Mat4x4 wvp;
  CameraNode *pCam = m_pScene->GetCamera();
  
  m_pGBufferShader->SetFloat("g_zfar", pCam->GetViewDistance());

  m_pGBufferShader->Begin();
  for(UINT i = 0; i < m_meshes.size(); ++i)
  {
    // Set world matrix since its needed for normals.
    m_pGBufferShader->SetMatrix("g_world", m_meshes[i]->world);

    // Save the wvp matrix to save some muls in vertex shader.
    wvp = m_meshes[i]->world * pCam->GetViewMatrix() * pCam->GetProjectionMatrix();
    
    m_pGBufferShader->SetMatrix("g_WVP", wvp);

    // Begin rendering using the single pass.
    m_pGBufferShader->BeginPass(0);
       
    // Render the mesh.
    m_meshes[i]->data.pMesh->RenderMesh();

    // Stop this pass.
    m_pGBufferShader->EndPass();
  }
  m_pGBufferShader->End();

  HR(pDevice->EndScene());

  m_pDepthTarget->Deactivate();
  // Deactivate rendertarget, which sets the current target to the backbuffer.
  m_pNormalTarget->Deactivate();
}

void SceneView::RenderLightPass()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  m_pDiffuseTarget->Activate(0);
  m_pSpecularTarget->Activate(1);

  m_lightDeclaration.Activate();

  //HR(pDevice->SetStreamSource(0, m_pScreenVBuffer, 0, sizeof(ScreenVertexData)));

  CameraNode *pCam = m_pScene->GetCamera();

  Vector2 halfPixel(0.5f / (float)m_pProvider->GetDisplayMode().width,
    0.5f / (float)m_pProvider->GetDisplayMode().height);

  m_pLightShader->SetVector2("g_halfPixel", halfPixel);
  m_pLightShader->SetMatrix("g_invProjection", pCam->GetProjectionMatrix().Inverse());
  m_pLightShader->SetVector3("g_cameraPosition", pCam->GetTransform().GetPosition());
  m_pLightShader->SetTexture("g_normalMap", m_pNormalTarget->GetTexture());

  // Clear render target.
  HR(pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0f, 0));

  HR(pDevice->BeginScene());

  for(size_t i = 0; i < m_lights.size(); ++i)
  {
    RenderLight(m_lights[i]->data.pLight); 
  }
  HR(pDevice->EndScene());

  m_pSpecularTarget->Deactivate();
  m_pDiffuseTarget->Deactivate();
}

void SceneView::RenderLight(const Light *pLight)
{
  // Set the active technique to the light MRT tech.
  m_pLightShader->SetActiveTechnique("LightMRTTech");

  // Set shader variable for the light.
  m_pLightShader->SetRaw("g_light", (void*)pLight, sizeof(Light));

  m_pLightShader->SetMatrix("g_WVP", CalcLightMatrix(pLight));

  switch(pLight->type)
  {
  case LightType::LT_DIRECTIONAL:
    m_pLightShader->SetActiveTechnique("DiffuseLightTech");
    // Begin rendering with technique.
    m_pLightShader->Begin();
    m_pLightShader->BeginPass(0);
    m_pQuadVolume->Render();
    break;
  case LightType::LT_POINT:
    m_pSphereVolume->Render();
    break;
  case LightType::LT_SPOT:
    m_pConeVolume->Render();
    break;
  default:
    assert(false && "Unknown LightType!");
  }

  m_pLightShader->EndPass();
  m_pLightShader->End();
}

Mat4x4 SceneView::CalcLightMatrix(const Light *pLight)
{
  CameraNode *pCam = m_pScene->GetCamera();

  if(pLight->type == LightType::LT_DIRECTIONAL)
  {
    return Mat4x4::CreateScale(2.0f);
  } 
  else if(pLight->type == LightType::LT_POINT)
  {

  }
  return Mat4x4::kIdentity;
}

void SceneView::RenderCombinedScene()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  // Normal voxel vertexdeclaration.
  m_voxelDeclaration.Activate();

  m_pCombineShader->SetTexture("g_diffuseMap", m_pDiffuseTarget->GetTexture());
  m_pCombineShader->SetTexture("g_specularMap", m_pSpecularTarget->GetTexture());

  // Clear render target.
  HR(pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
    0, 1.0f, 0));

  // Begin rendering.
  HR(pDevice->BeginScene());

  m_pCombineShader->SetActiveTechnique("CombineTech");
  CameraNode *pCam = m_pScene->GetCamera();
  Mat4x4 wvp;  
  m_pCombineShader->Begin();
  for(UINT i = 0; i < m_meshes.size(); ++i)
  {
    wvp = m_meshes[i]->world * pCam->GetViewMatrix() * pCam->GetProjectionMatrix();

    m_pCombineShader->SetMatrix("g_WVP", wvp);
    m_pCombineShader->BeginPass(0);
    m_meshes[i]->data.pMesh->RenderMesh();
    m_pCombineShader->EndPass();
  }
  m_pCombineShader->End();

  HR(pDevice->EndScene());

  //m_pCombineShader->SetTexture("g_diffuseMap", nullptr);
  //m_pCombineShader->SetTexture("g_specularMap", nullptr);
}

void SceneView::DisplayRenderTarget(const RenderTarget *pTarget)
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  m_textureDeclaration.Activate();

  HR(pDevice->BeginScene());

  // Draw output.
  m_pCombineShader->SetActiveTechnique("RenderToScreen");

  HR(pDevice->SetStreamSource(0, m_pScreenVBuffer, 0, sizeof(ScreenVertexData)));
  m_pCombineShader->SetTexture("g_texture", pTarget->GetTexture());

  m_pCombineShader->Begin();
  m_pCombineShader->BeginPass(0);
  pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  m_pCombineShader->EndPass();
  m_pCombineShader->End();

  HR(pDevice->EndScene());
}

void SceneView::OnDeviceLost()
{
  m_pNormalTarget->OnDeviceLost();
  m_pDiffuseTarget->OnDeviceLost();
  m_pSpecularTarget->OnDeviceLost();
  m_pDepthTarget->OnDeviceLost();

  m_voxelDeclaration.OnDeviceLost();
  m_textureDeclaration.OnDeviceLost();
  m_lightDeclaration.OnDeviceLost();
}

void SceneView::OnDeviceReset()
{
  // Create Vertexbuffer used to draw rendertargets to screen.
  CreateScreenVBuffer();

  m_pNormalTarget->OnDeviceReset();
  m_pDiffuseTarget->OnDeviceReset();
  m_pSpecularTarget->OnDeviceReset();
  m_pDepthTarget->OnDeviceReset();

  m_voxelDeclaration.OnDeviceReset();
  m_textureDeclaration.OnDeviceReset();
  m_lightDeclaration.OnDeviceReset();
}

void SceneView::CreateVertexDecl()
{
  m_textureDeclaration.AddElement(
    VertexElement(VertexDataType::VDT_FLOAT4, VertexUsageType::VUT_POSITION, 0)
  );

  m_textureDeclaration.AddElement(
    VertexElement(VertexDataType::VDT_FLOAT2, VertexUsageType::VUT_TEXCOORDS, 0)
  );

  m_voxelDeclaration.AddElement(
    VertexElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_POSITION, 0)
  );

  m_voxelDeclaration.AddElement(
    VertexElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_NORMAL, 0)
  );

  m_voxelDeclaration.AddElement(
    VertexElement(VertexDataType::VDT_FLOAT4, VertexUsageType::VUT_COLOR, 0)
  );

  m_voxelDeclaration.AddElement(
    VertexElement(VertexDataType::VDT_FLOAT4, VertexUsageType::VUT_COLOR, 1)
  );

  m_lightDeclaration.AddElement(
    VertexElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_POSITION, 0)
  );
}

void SceneView::CreateScreenVBuffer()
{
  RELEASECOM(m_pScreenVBuffer);
  HR(m_pProvider->GetDevice()->CreateVertexBuffer(
    sizeof(ScreenVertexData) * kScreenVertCount,
    D3DUSAGE_WRITEONLY,
    0,
    D3DPOOL_MANAGED,
    &m_pScreenVBuffer,
    NULL
  ));
    
  ScreenVertexData *pData;
  HR(m_pScreenVBuffer->Lock(0, 0, (void**)&pData, 0));
  for(int i = 0; i < kScreenVertCount; ++i)
  {
    pData[i] = kScreenVertices[i];
  }

  HR(m_pScreenVBuffer->Unlock());
}

} // namespace shinybear