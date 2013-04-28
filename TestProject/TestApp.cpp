#include "TestApp.h"
#include <base\system\GraphicsProvider.h>
#include <scene\SceneManager.h>
#include <scene\SceneView.h>
#include <scene\MeshNode.h>
#include <scene\LightNode.h>
#include <graphics\Mesh.h>

using namespace shinybear;

TestApp::TestApp() 
{

}

TestApp::~TestApp()
{
  delete m_pScene;
  delete m_pView;
}

bool TestApp::OnInitialize() 
{
  m_pScene = new SceneManager(GetGraphicsProvider());
  m_pView = new SceneView(GetGraphicsProvider(), m_pScene);

  Mesh *pMesh = new Mesh(GetGraphicsProvider());

  BlockMaterial redMat;
  redMat.diffuse = 0xFFFF;
  redMat.specular = 0xFFFFFF;

  D3DXVECTOR3 normal(0, 0, -1.0f);
  D3DXVECTOR3 tl(-0.5f, +0.5f, 5.0f);
  D3DXVECTOR3 tr(+0.5f, +0.5f, 5.0f);
  D3DXVECTOR3 br(+0.5f, -0.5f, 5.0f);
  D3DXVECTOR3 bl(-0.5f, -0.5f, 5.0f);

  shinybear::UINT v0, v1, v2, v3;

  v0 = pMesh->AddVertex(tl, redMat, normal); 
  v1 = pMesh->AddVertex(tr, redMat, normal);
  v2 = pMesh->AddVertex(br, redMat, normal);
  v3 = pMesh->AddVertex(bl, redMat, normal);

  pMesh->AddTriangle(v0, v1, v2);
  pMesh->AddTriangle(v2, v3, v0);
  pMesh->AddTriangle(v2, v1, v0);
  pMesh->AddTriangle(v2, v1, v3);

  pMesh->UpdateBuffers();

  m_pMeshNode = new MeshNode(pMesh);

  Light *pLight = new AmbientLight();
  LightNode *pLightNode = new LightNode(pLight);
  m_pScene->GetRoot()->Attach(pLightNode);

  m_pScene->GetRoot()->Attach(m_pMeshNode);

  //m_pMeshNode->Scale(10.0f);


  return true;
}


void TestApp::OnUpdate(double elapsedSeconds) 
{
  BaseGame::OnUpdate(elapsedSeconds);
  m_pScene->Update(elapsedSeconds);

  m_pMeshNode->Rotate(0, 0, 1.0f);
}

void TestApp::OnRender()
{
  GetGraphicsProvider()->Clear(Color4f(0.25f, 0.25f, 0.25f, 0.25f));
  m_pView->Render();
}


