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
  { Vector4(-1, +1, 0, 1), Vector2(0, 0) },
  { Vector4(+1, +1, 0, 1), Vector2(1, 0) },
  { Vector4(-1, -1, 0, 1), Vector2(0, 1) },
  { Vector4(+1, -1, 0, 1), Vector2(1, 1) },
};

SceneView::SceneView(GraphicsProvider *pProvider, SceneManager *pScene)
{
  m_pScene = pScene;
  m_pProvider = pProvider;
  m_isRendering = false;
  m_pShader = nullptr;
  m_pDecl = nullptr;
  m_pTextureDecl = nullptr;
  m_pScreenVBuffer = nullptr;

  GetAbsolutePath(L"res\\shaders\\LightShader.fx", &m_pShaderPath);

  m_pNormalTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A8R8G8B8);
  m_pDiffuseTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A8R8G8B8);
  m_pSpecularTarget = DBG_NEW RenderTarget(pProvider, D3DFMT_A8R8G8B8);

  OnDeviceReset();
}

SceneView::~SceneView()
{
  delete[] m_pShaderPath;
  delete m_pNormalTarget;
  delete m_pDiffuseTarget;
  delete m_pSpecularTarget;
  RELEASECOM(m_pDecl);
  RELEASECOM(m_pTextureDecl);
  RELEASECOM(m_pShader);
  RELEASECOM(m_pScreenVBuffer);
}

void SceneView::Render(const RenderList &list)
{
  assert(!m_isRendering && "Sceneview already rendering!");
  m_isRendering = true;

  m_lights.clear();
  m_meshes.clear();  

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
  m_pShader->SetMatrix(m_hFxView, &pCam->GetViewMatrix());
  m_pShader->SetMatrix(m_hFxProjection, &pCam->GetProjectionMatrix());

  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();  
  
  pDevice->EndScene();

  // Render N/M Buffer.
  RenderNormalPass();

  RenderLightPass();

  // 
  DisplayRenderTarget(m_pDiffuseTarget);

  m_isRendering = false;
}

void SceneView::RenderNormalPass()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();
  
  // Activate the render target for normals, in slot 0.
  m_pNormalTarget->Activate(0);
  
  // Normal voxel vertexdeclaration.
  HR(pDevice->SetVertexDeclaration(m_pDecl));

  // Clear render target.
  HR(pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
    0, 1.0f, 0));

  // Begin rendering.
  HR(pDevice->BeginScene());

  // Render geometry using normal-specular technique.
  RenderGeometry(m_hFxNormalTech);

  pDevice->EndScene();

  // Deactivate rendertarget, which sets the current target to the backbuffer.
  m_pNormalTarget->Deactivate();
}

void SceneView::RenderLightPass()
{
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  m_pDiffuseTarget->Activate(0);
  m_pSpecularTarget->Activate(1);


  HR(pDevice->SetVertexDeclaration(m_pTextureDecl));
  HR(pDevice->SetStreamSource(0, m_pScreenVBuffer, 0, sizeof(ScreenVertexData)));

  CameraNode *pCam = m_pScene->GetCamera();
  
  HR(m_pShader->SetRawValue(m_hFxCamPos,
    (void*)(&pCam->GetTransform().GetPosition()), 0, sizeof(Vector3)));

  HR(m_pShader->SetTexture(m_hFxNormalMap, m_pNormalTarget->GetTexture()));

  // Clear render target.
  HR(pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
    0, 1.0f, 0));

  HR(pDevice->BeginScene());

  HR(m_pShader->SetTechnique(m_hFxLightMRTTech));
  
  UINT numPasses;
  HR(m_pShader->Begin(&numPasses, 0));
  for(size_t li = 0; li < m_lights.size(); ++li)
  {
    HR(m_pShader->SetRawValue(m_hFxLight, (void*)m_lights[li]->data.pLight, 0, sizeof(Light)));
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

  HR(pDevice->SetVertexDeclaration(m_pTextureDecl));
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
}

void SceneView::OnDeviceLost()
{
  RELEASECOM(m_pDecl);
  RELEASECOM(m_pShader);

  m_pNormalTarget->OnDeviceLost();
  m_pDiffuseTarget->OnDeviceLost();
  m_pSpecularTarget->OnDeviceLost();
}

void SceneView::OnDeviceReset()
{
  // Create Vertex Declarations.
  CreateVertexDecl();

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

  // Retrieve handles to our techniques.
  m_hFxNormalTech = m_pShader->GetTechniqueByName("NormalTech");
  m_hFxGeometryTech = m_pShader->GetTechniqueByName("GeometryTech");
  m_hFxRenderScreenTech = m_pShader->GetTechniqueByName("RenderToScreen");
  m_hFxLightMRTTech = m_pShader->GetTechniqueByName("LightPassMRT");

  // Create Vertexbuffer used to draw rendertargets to screen.
  CreateScreenVBuffer();

  m_pNormalTarget->OnDeviceReset();
  m_pDiffuseTarget->OnDeviceReset();
  m_pSpecularTarget->OnDeviceReset();
}

void SceneView::CreateVertexDecl()
{
  // For easier access.
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice();

  // Vertex declaration used for voxels.
  D3DVERTEXELEMENT9 elements[] = 
  {
    { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
    { 0, 24, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
    { 0, 40, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1 },
    D3DDECL_END()
  };
  RELEASECOM(m_pDecl);
  HR(pDevice->CreateVertexDeclaration(elements, &m_pDecl));

  
  // Declaration used for drawing rendertargets to screen.
  D3DVERTEXELEMENT9 tElements[] = 
  {
    { 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
  };

  RELEASECOM(m_pTextureDecl);
  HR(pDevice->CreateVertexDeclaration(tElements, &m_pTextureDecl));
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