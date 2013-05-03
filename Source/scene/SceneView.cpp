#include "scene\SceneView.h"
#include "scene\SceneManager.h"
#include "scene\SceneNode.h"
#include "scene\CameraNode.h"
#include "base\system\GraphicsProvider.h"
#include "graphics\Mesh.h"
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
  :m_voxelDeclaration(pProvider), m_textureDeclaration(pProvider)
{
  m_pScene = pScene;
  m_pProvider = pProvider;
  m_isRendering = false;
  m_pShader = nullptr;
  m_pScreenVBuffer = nullptr;

  GetAbsolutePath(L"res\\shaders\\LightShader.fx", &m_pShaderPath);

  m_pNormalTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A16B16G16R16F);
  m_pDiffuseTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A16B16G16R16F);
  m_pSpecularTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A16B16G16R16F);
  m_pDepthTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_R32F);

  CreateVertexDecl();

  OnDeviceReset();
}

SceneView::~SceneView()
{
  delete[] m_pShaderPath;
  delete m_pNormalTarget;
  delete m_pDiffuseTarget;
  delete m_pSpecularTarget;
  delete m_pDepthTarget;
  RELEASECOM(m_pShader);
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
  for(int i = 0; i < list.size(); ++i)
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
  pCam->Update(0);
  m_pShader->SetMatrix(m_hFxView, &pCam->GetViewMatrix());
  m_pShader->SetMatrix(m_hFxProjection, &pCam->GetProjectionMatrix());
  m_pShader->SetFloat(m_hFxNearPlane, pCam->GetNearPlane());
  m_pShader->SetFloat(m_hFxFarPlane, pCam->GetViewDistance());

  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();  
  
  pDevice->EndScene();

  // Render N/M Buffer.
  RenderNormalPass();

  RenderLightPass();

  RenderCombinedScene();

  // 
  //DisplayRenderTarget(m_pNormalTarget);

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

  // Render geometry using normal-specular technique.
  RenderGeometry(m_hFxNormalTech);

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

  m_textureDeclaration.Activate();

  HR(pDevice->SetStreamSource(0, m_pScreenVBuffer, 0, sizeof(ScreenVertexData)));

  CameraNode *pCam = m_pScene->GetCamera();
  
  HR(m_pShader->SetRawValue(m_hFxCamPos,
    (void*)(&pCam->GetTransform().GetPosition()), 0, sizeof(Vector3)));

  HR(m_pShader->SetTexture(m_hFxNormalMap, m_pNormalTarget->GetTexture()));

  // Clear render target.
  HR(pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0, 1.0f, 0));

  HR(pDevice->BeginScene());

  HR(m_pShader->SetTechnique(m_hFxLightMRTTech));
  
  UINT numPasses;
  HR(m_pShader->Begin(&numPasses, 0));
  for(size_t li = 0; li < m_lights.size(); ++li)
  {
    HR(m_pShader->SetRawValue(
      m_hFxLight,
      (void*)m_lights[li]->data.pLight,
      0,
      sizeof(Light)
    ));

    for(UINT i = 0; i < numPasses; ++i)
    {
      HR(m_pShader->BeginPass(i));
      HR(pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2));
      HR(m_pShader->EndPass());
    }
  }
  HR(m_pShader->End());

  HR(pDevice->EndScene());

  m_pSpecularTarget->Deactivate();
  m_pDiffuseTarget->Deactivate();
}


void SceneView::RenderCombinedScene()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  // Normal voxel vertexdeclaration.
  m_voxelDeclaration.Activate();

  HR(m_pShader->SetTexture(m_hFxDiffuseMap, m_pDiffuseTarget->GetTexture()));
  HR(m_pShader->SetTexture(m_hFxSpecularMap, m_pSpecularTarget->GetTexture()));

  // Clear render target.
  HR(pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
    0, 1.0f, 0));

  // Begin rendering.
  HR(pDevice->BeginScene());

  RenderGeometry(m_hFxCombineTech);

  HR(pDevice->EndScene());
}

void SceneView::RenderGeometry(D3DXHANDLE hTech)
{
  // Enable the normal pass rendering technique.
  HR(m_pShader->SetTechnique(hTech));

  UINT passCount;

  // Begin rendering using the current technique.
  HR(m_pShader->Begin(&passCount, 0));
  for(size_t mi = 0; mi < m_meshes.size(); ++mi)
  {
    // Set world matrix.
    HR(m_pShader->SetMatrix(m_hFxWorld, &m_meshes[mi]->world));
    for(int i = 0; i < passCount; ++i)
    {
      HR(m_pShader->BeginPass(i));
      m_meshes[mi]->data.pMesh->RenderMesh();
      HR(m_pShader->EndPass());
    }
  }
  // Stop rendering using the current technique.
  HR(m_pShader->End());
}

void SceneView::DisplayRenderTarget(const RenderTarget *pTarget)
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  m_textureDeclaration.Activate();

  HR(pDevice->BeginScene());

  // Draw output.
  HR(m_pShader->SetTechnique(m_hFxRenderScreenTech));

  HR(pDevice->SetStreamSource(0, m_pScreenVBuffer, 0, sizeof(ScreenVertexData)));

  HR(m_pShader->SetTexture(m_hFxTexture, pTarget->GetTexture()));

  UINT numPasses;
  HR(m_pShader->Begin(&numPasses, 0));
  HR(m_pShader->BeginPass(0));
  pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  HR(m_pShader->EndPass());
  HR(m_pShader->End());

  //HR(pDevice->EndScene());
}

void SceneView::OnDeviceLost()
{
  RELEASECOM(m_pShader);

  m_pNormalTarget->OnDeviceLost();
  m_pDiffuseTarget->OnDeviceLost();
  m_pSpecularTarget->OnDeviceLost();
  m_pDepthTarget->OnDeviceLost();

  m_voxelDeclaration.OnDeviceLost();
  m_textureDeclaration.OnDeviceLost();
}

void SceneView::OnDeviceReset()
{
  // Reloads the shader.
  LoadShader();

  // Retrieve handles to our shader variables.
  m_hFxWorld = m_pShader->GetParameterByName(0, "g_world");
  m_hFxView = m_pShader->GetParameterByName(0, "g_view");
  m_hFxProjection = m_pShader->GetParameterByName(0, "g_projection");
  m_hFxTexture = m_pShader->GetParameterByName(0, "g_texture");
  m_hFxLight = m_pShader->GetParameterByName(0, "g_light");
  m_hFxCamPos = m_pShader->GetParameterByName(0, "g_cameraPosition");
  m_hFxNormalMap = m_pShader->GetParameterByName(0, "g_normalMap");
  m_hFxFarPlane = m_pShader->GetParameterByName(0, "g_zfar");
  m_hFxNearPlane = m_pShader->GetParameterByName(0, "g_znear");
  m_hFxDiffuseMap = m_pShader->GetParameterByName(0, "g_diffuseMap");
  m_hFxSpecularMap = m_pShader->GetParameterByName(0, "g_specularMap");

  // Retrieve handles to our techniques.
  m_hFxNormalTech = m_pShader->GetTechniqueByName("NormalTech");
  m_hFxGeometryTech = m_pShader->GetTechniqueByName("GeometryTech");
  m_hFxRenderScreenTech = m_pShader->GetTechniqueByName("RenderToScreen");
  m_hFxLightMRTTech = m_pShader->GetTechniqueByName("LightPassMRT");
  m_hFxCombineTech = m_pShader->GetTechniqueByName("CombineTech");

  // Create Vertexbuffer used to draw rendertargets to screen.
  CreateScreenVBuffer();

  m_pNormalTarget->OnDeviceReset();
  m_pDiffuseTarget->OnDeviceReset();
  m_pSpecularTarget->OnDeviceReset();
  m_pDepthTarget->OnDeviceReset();

  m_voxelDeclaration.OnDeviceReset();
  m_textureDeclaration.OnDeviceReset();
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
}

void SceneView::LoadShader()
{
  ID3DXBuffer *pBuffer;

  D3DXCreateEffectFromFileW(
    m_pProvider->GetDevice(),
    m_pShaderPath,
    0,
    0,
    D3DXSHADER_DEBUG,
    0,
    &m_pShader,
    &pBuffer
  );

  if(pBuffer)
  {
    OutputDbgFormat("SHADER ERRORS: %s\n", (char*)pBuffer->GetBufferPointer());
  }
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