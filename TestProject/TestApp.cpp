#include "TestApp.h"
#include <base\system\GraphicsProvider.h>
#include <scene\SceneManager.h>
#include <scene\SceneView.h>
#include <scene\MeshNode.h>
#include <scene\LightNode.h>
#include <graphics\Mesh.h>
#include <world\Block.h>

using namespace shinybear;

TestApp::TestApp() 
{

}

TestApp::~TestApp()
{
  delete m_pScene;
}

// REMOVE ME
void CreateCube(float x, float y, float z, const Block &block,
  Mesh *pMesh, int hideFlags)
{
  static const D3DXVECTOR3 kFrontNormal   = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
  static const D3DXVECTOR3 kBackNormal    = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
  static const D3DXVECTOR3 kTopNormal     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
  static const D3DXVECTOR3 kBottomNormal  = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
  static const D3DXVECTOR3 kLeftNormal    = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
  static const D3DXVECTOR3 kRightNormal   = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

  // Front positions.
  const D3DXVECTOR3 p0 = D3DXVECTOR3(x - 0.5f, y + 0.5f, z - 0.5f);
  const D3DXVECTOR3 p1 = D3DXVECTOR3(x + 0.5f, y + 0.5f, z - 0.5f);
  const D3DXVECTOR3 p2 = D3DXVECTOR3(x + 0.5f, y - 0.5f, z - 0.5f);
  const D3DXVECTOR3 p3 = D3DXVECTOR3(x - 0.5f, y - 0.5f, z - 0.5f);

  // Back Positions
  const D3DXVECTOR3 p4 = D3DXVECTOR3(x + 0.5f, y + 0.5f, z + 0.5f);
  const D3DXVECTOR3 p5 = D3DXVECTOR3(x - 0.5f, y + 0.5f, z + 0.5f);
  const D3DXVECTOR3 p6 = D3DXVECTOR3(x - 0.5f, y - 0.5f, z + 0.5f);
  const D3DXVECTOR3 p7 = D3DXVECTOR3(x + 0.5f, y - 0.5f, z + 0.5f);

  shinybear::UINT v0, v1, v2, v3;
  BlockMaterial mat = block.GetMaterial();

  v0 = pMesh->AddVertex(p0, mat, kFrontNormal);
  v1 = pMesh->AddVertex(p1, mat, kFrontNormal);
  v2 = pMesh->AddVertex(p2, mat, kFrontNormal);
  v3 = pMesh->AddVertex(p3, mat, kFrontNormal);
    
  pMesh->AddTriangle(v0, v1, v2);
  pMesh->AddTriangle(v2, v3, v0);
  


  v0 = pMesh->AddVertex(p4, mat, kBackNormal);
  v1 = pMesh->AddVertex(p5, mat, kBackNormal);
  v2 = pMesh->AddVertex(p6, mat, kBackNormal);
  v3 = pMesh->AddVertex(p7, mat, kBackNormal);
    
  pMesh->AddTriangle(v0, v1, v2);
  pMesh->AddTriangle(v2, v3, v0);
  

 
  v0 = pMesh->AddVertex(p5, mat, kTopNormal);
  v1 = pMesh->AddVertex(p4, mat, kTopNormal);
  v2 = pMesh->AddVertex(p1, mat, kTopNormal);
  v3 = pMesh->AddVertex(p0, mat, kTopNormal);
    
  pMesh->AddTriangle(v0, v1, v2);
  pMesh->AddTriangle(v2, v3, v0);
  


  v0 = pMesh->AddVertex(p3, mat, kBottomNormal);
  v1 = pMesh->AddVertex(p2, mat, kBottomNormal);
  v2 = pMesh->AddVertex(p7, mat, kBottomNormal);
  v3 = pMesh->AddVertex(p6, mat, kBottomNormal);
    
  pMesh->AddTriangle(v0, v1, v2);
  pMesh->AddTriangle(v2, v3, v0);
  


  v0 = pMesh->AddVertex(p5, mat, kLeftNormal);
  v1 = pMesh->AddVertex(p0, mat, kLeftNormal);
  v2 = pMesh->AddVertex(p3, mat, kLeftNormal);
  v3 = pMesh->AddVertex(p6, mat, kLeftNormal);
    
  pMesh->AddTriangle(v0, v1, v2);
  pMesh->AddTriangle(v2, v3, v0);
  


  v0 = pMesh->AddVertex(p1, mat, kRightNormal);
  v1 = pMesh->AddVertex(p4, mat, kRightNormal);
  v2 = pMesh->AddVertex(p7, mat, kRightNormal);
  v3 = pMesh->AddVertex(p2, mat, kRightNormal);
    
  pMesh->AddTriangle(v0, v1, v2);
  pMesh->AddTriangle(v2, v3, v0);
  
}

bool TestApp::OnInitialize() 
{
  m_pScene = new SceneManager(GetGraphicsProvider());

 

  BlockMaterial redMat;
  redMat.diffuse = 0xFFFF0000;
  redMat.specular = 0xFFFFFFFF;
  Block block;
  block.SetMaterial(redMat);
  block.SetVisible(true);
  block.SetType(0);

  //Light *pLight = new AmbientLight();
  //LightNode *pLightNode = new LightNode(pLight);
  //m_pScene->GetRoot()->Attach(pLightNode);

  Mesh *pMesh = new Mesh(GetGraphicsProvider());

  
  CreateCube(0, 0, 0, block, pMesh, 0);
  pMesh->UpdateBuffers();
  m_pMeshNode = new MeshNode(pMesh);

  Mesh *pOther = new Mesh(GetGraphicsProvider());

  redMat.diffuse = 0xFF00FF00;
  block.SetMaterial(redMat);

  CreateCube(0, 0, 0, block, pOther, 0);
  pOther->UpdateBuffers();
  m_pOtherNode = new MeshNode(pOther);

  Mesh *pThird = new Mesh(GetGraphicsProvider());
  CreateCube(0, 0, 0, block, pThird, 0);
  pThird->UpdateBuffers();
  m_pThirdNode = new MeshNode(pThird);

  //m_pScene->GetRoot()->Translate(5.0f, 0, 0);

  m_pScene->GetRoot()->Attach(m_pMeshNode);

  m_pOtherNode->Translate(0, 4.0f, 0);

  m_pMeshNode->Attach(m_pOtherNode);

  //m_pMeshNode->Translate(0, 2.0, 0.0f);

  //m_pMeshNode->Scale(0.5f);

  m_pOtherNode->Attach(m_pThirdNode);
  m_pOtherNode->Scale(0.25f);


  m_pThirdNode->Translate(4.0f, 0, 0);
  //pThirdNode->Rotate(0, 0, 25.0f);

  m_pThirdNode->Scale(2.75f);
  return true;
}

float rotA = 0.0f;
float rotB = 0.0f;

void TestApp::OnUpdate(double elapsedSeconds) 
{
  BaseGame::OnUpdate(elapsedSeconds);
  m_pScene->Update(elapsedSeconds);

  rotA += -1.0f * elapsedSeconds;
  rotB += 2.5f * elapsedSeconds;

  m_pMeshNode->Rotate(rotB, 0, rotA);
  m_pOtherNode->Rotate(0, 0, rotB);
  m_pThirdNode->Rotate(0, rotB, 0);
  //m_pMeshNode->Translate(1.0f * elapsedSeconds, 0, 0);
}

void TestApp::OnRender()
{
  GetGraphicsProvider()->Clear(Color4f(0.25f, 0.25f, 0.25f, 0.25f));
  m_pScene->Render();
}


