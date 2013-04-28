#include "scene\SceneView.h"
#include "scene\SceneManager.h"
#include "scene\SceneNode.h"
#include "base\system\GraphicsProvider.h"
#include "graphics\Mesh.h"
#include "util\SBUtil.h"
#include <cassert>
#include <d3dx9.h>

namespace shinybear
{

SceneView::SceneView(GraphicsProvider *pProvider, SceneManager *pScene)
{
  m_pScene = pScene;
  m_pProvider = pProvider;
  m_isRendering = false;
  m_pShader = nullptr;

  ID3DXBuffer *pBuffer;

  GetAbsolutePath(L"res\\shaders\\LightShader.fx", &m_pShaderPath);
  D3DXCreateEffectFromFileW(
    pProvider->GetDevice(),
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

  D3DVERTEXELEMENT9 elements[] = 
  {
    { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
    { 0, 24, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
    { 0, 40, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1 },
    D3DDECL_END()
  };

  HR(m_pProvider->GetDevice()->CreateVertexDeclaration(elements, &m_pDecl));
}

SceneView::~SceneView()
{
  delete[] m_pShaderPath;
}

void SceneView::Render()
{
  assert(!m_isRendering && "Sceneview already rendering!");
  m_isRendering = true;

  m_lights.clear();
  m_meshes.clear();

  // Let nodes of the scenegraph fill our information.
  m_pScene->GetRoot()->Render(this);

  D3DXMATRIX view;
  D3DXVECTOR3 eye(0, 0, -10.0f);
  D3DXVECTOR3 at(0, 0, 0);
  D3DXVECTOR3 up(0, 1.0f, 0);
  D3DXMatrixLookAtLH(&view, &eye, &at, &up);

  D3DXMATRIX proj;
  D3DXMatrixPerspectiveFovLH(&proj,
    60.0f * (3.141592 / 180.f), 1280.0f / 768.0f, 0.1f, 1000.0f);

  D3DXHANDLE hWorld = m_pShader->GetParameterByName(0, "g_world");
  D3DXHANDLE hView = m_pShader->GetParameterByName(0, "g_view");
  D3DXHANDLE hProj = m_pShader->GetParameterByName(0, "g_projection");

  HR(m_pProvider->GetDevice()->SetVertexDeclaration(m_pDecl));

  HR(m_pProvider->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));

  D3DXHANDLE hTech = m_pShader->GetTechniqueByName("BaseTech");
  HR(m_pShader->SetTechnique(hTech));

  
  m_pShader->SetMatrix(hView, &view);
  m_pShader->SetMatrix(hProj, &proj);

  UINT numPasses;
  HR(m_pShader->Begin(&numPasses, 0));
  for(int i = 0; i < m_meshes.size(); ++i)
  {
    HR(m_pShader->SetMatrix(hWorld, &m_meshes[i]->GetTransform()));
    HR(m_pShader->BeginPass(0));
    m_meshes[i]->GetMesh()->RenderMesh();
    HR(m_pShader->EndPass());
  }
  HR(m_pShader->End());

  m_isRendering = false;
}

void SceneView::AddMeshNode(MeshNode *pMesh)
{
  m_meshes.push_back(pMesh);
}

void SceneView::AddLight(LightNode *pLight)
{
  m_lights.push_back(pLight);
}

void SceneView::OnDeviceLost()
{

}

void SceneView::OnDeviceReset()
{

}

} // namespace shinybear