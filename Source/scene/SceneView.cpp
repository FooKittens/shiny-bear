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
  RELEASECOM(m_pDecl);
  RELEASECOM(m_pShader);
}

void SceneView::Render(const RenderList &list)
{
  assert(!m_isRendering && "Sceneview already rendering!");
  m_isRendering = true;

  m_lights.clear();
  m_meshes.clear();

  D3DXMATRIX view;
  D3DXVECTOR3 eye(0, 0, -10.0f);
  D3DXVECTOR3 at(0, 0, 0);
  D3DXVECTOR3 up(0, 1.0f, 0);
  D3DXMatrixLookAtLH(&view, &eye, &at, &up);

  D3DXMATRIX proj;
  D3DXMatrixPerspectiveFovLH(&proj,
    60.0f * (3.141592 / 180.f), 1280.0f / 768.0f, 1.0f, 1000.0f);

  D3DXHANDLE hWorld = m_pShader->GetParameterByName(0, "g_world");
  D3DXHANDLE hView = m_pShader->GetParameterByName(0, "g_view");
  D3DXHANDLE hProj = m_pShader->GetParameterByName(0, "g_projection");

  HR(m_pProvider->GetDevice()->SetVertexDeclaration(m_pDecl));
  D3DXHANDLE hTech = m_pShader->GetTechniqueByName("BaseTech");
  
  //D3DVIEWPORT9 vp;
  //vp.Height = 768;
  //vp.Width = 1280;
  //vp.MaxZ = 1.0f;
  //vp.MinZ = 0.0f;
  //vp.X = 0;
  //vp.Y = 0;
  //
  //HR(m_pProvider->GetDevice()->SetViewport(&vp));

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


  m_pShader->SetMatrix(hView, &view);
  m_pShader->SetMatrix(hProj, &proj);

  HR(m_pProvider->GetDevice()->SetVertexDeclaration(m_pDecl));
  HR(m_pShader->SetTechnique(hTech));
  
  UINT numPasses;
  //m_pProvider->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
  HR(m_pShader->Begin(&numPasses, 0));
  for(int i = 0; i < m_meshes.size(); ++i)
  {
    HR(m_pShader->SetMatrix(hWorld, &m_meshes[i]->world));
    HR(m_pShader->BeginPass(0));
    
    m_meshes[i]->data.pMesh->RenderMesh();

    HR(m_pShader->EndPass());
  }
  HR(m_pShader->End());

  m_isRendering = false;
}


void SceneView::OnDeviceLost()
{

}

void SceneView::OnDeviceReset()
{

}

} // namespace shinybear