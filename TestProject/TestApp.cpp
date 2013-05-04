#include "TestApp.h"
#include <base\system\GraphicsProvider.h>
#include <util\input\InputManager.h>
#include <util\input\GamePadState.h>
#include <util\input\KeyboardState.h>
#include <util\input\MouseState.h>
#include <scene\SceneManager.h>
#include <scene\SceneView.h>
#include <scene\MeshNode.h>
#include <scene\LightNode.h>
#include <scene\CameraNode.h>
#include <graphics\Mesh.h>
#include <graphics\Light.h>
#include <world\Block.h>
#include <world\Cluster.h>
#include <util\SBUtil.h>

using namespace shinybear;

TestApp::TestApp() 
{
  m_pScene = nullptr;
  m_pMeshNode = nullptr;
  m_pOtherNode = nullptr;
}

TestApp::~TestApp()
{
  //for(int i = 0; i < m_meshes.size(); ++i)
  //{
  //  delete m_meshes[i];
  //}

  //m_meshes.clear();

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

MeshNode *TestApp::CreateMeshNode(BlockMaterial *mat)
{
  static Block block;
  block.SetMaterial(*mat);

  Mesh *pMesh = DBG_NEW Mesh(GetGraphicsProvider());
  MeshNode *pNode = DBG_NEW MeshNode(pMesh);

  pMesh->BeginMesh();
  CreateCube(0, 0, 0, block, pMesh, 0);
  pMesh->EndMesh();
  return pNode;
}

Light light;
Light light2;

bool TestApp::OnInitialize() 
{
  m_pScene = DBG_NEW SceneManager(GetGraphicsProvider());


  BlockMaterial redMat;
  redMat.diffuse = 0xFF8800AA;
  redMat.specular = 0xFFFFFFFF;
  
#pragma region OLDSTUFF
  m_pMeshNode = CreateMeshNode(&redMat);

  redMat.diffuse = 0xFF8800AA;

  m_pOtherNode = CreateMeshNode(&redMat);
  m_pThirdNode = CreateMeshNode(&redMat);

  m_pScene->GetRoot()->Attach(m_pMeshNode);
  
  m_pMeshNode->Translate(0, 20.0f, -20.0f);
  m_pOtherNode->Translate(50.0f, 0.0f, 0.0f);
  
  m_pThirdNode->Attach(m_pOtherNode);

  m_pThirdNode->Scale(0.1f);
  m_pOtherNode->Scale(10.0f);

  m_pMeshNode->Attach(m_pThirdNode);


  for(int i = 0; i < 8; ++i)
    for(int k = 0; k < 8; ++k)
    {
      Cluster *cluster = DBG_NEW Cluster(GetGraphicsProvider());
      m_pScene->GetRoot()->Attach(cluster);
      cluster->Translate((i - 4) * 16, 0, (k - 4) * 16);
      //cluster->Rotate(0, 3.141592f / 2.0f, 0);
    }

  light = Light::CreateDirectionalLight(
    D3DXCOLOR(0.55f, 0.55f, 0.55f, 1.0f),
    Vector3(1.0f, -3.0f, 1.5f));

  light2 = Light::CreateDirectionalLight(
    D3DXCOLOR(0.15f, 0.85f, 0.15f, 1.0f),
    Vector3(1.0f, -1.0f, 0));

  LightNode *pLightNode = new LightNode(&light);
  LightNode *pLightNode2 = new LightNode(&light2);

 
  m_pScene->GetRoot()->Attach(pLightNode);
  m_pScene->GetRoot()->Attach(pLightNode2);
#pragma endregion


  CameraNode *pCam = new CameraNode(GetWindow(), m_pMeshNode);
  //CameraNode *pCam = new CameraNode(GetWindow(), m_pScene->GetRoot());

  m_pScene->SetCamera(pCam);

  return true;
}

float rotA = 0.0f;
float rotB = 0.0f;

GamePadState gamePad(GamePadIndex::ONE);
KeyboardState keys;

float x = 0.0f, y = 0.0f, z = 0.0f;
float xT = 0.0f;
float xDir = 1.0f;

void TestApp::OnUpdate(double elapsedSeconds) 
{
  BaseGame::OnUpdate(elapsedSeconds);
  m_pScene->Update(elapsedSeconds);

  m_pThirdNode->Rotate(2.5f * elapsedSeconds, 0, 0);
  

  InputManager::GetControllerState(&gamePad);
  InputManager::GetKeyboardState(&keys);

  rotA = 0.0f;
  z = 0.0f;
  x = 0.0f;
  y = 0.0f;

  rotA = 1.5f * elapsedSeconds * gamePad.rightThumbstick.x;

  z += 5.5f * elapsedSeconds * gamePad.leftTrigger;
  z -= 5.5f * elapsedSeconds * gamePad.rightTrigger;
  gamePad.Vibrate(gamePad.leftTrigger, gamePad.rightTrigger);
  if(keys.IsKeyDown(Keys::K_UP))
  {
    z -= 9.5f * elapsedSeconds;
  }
  if(keys.IsKeyDown(Keys::K_DOWN))
  {
    z += 9.5f * elapsedSeconds;
  }

  if(keys.IsKeyDown(Keys::K_LEFT))
  {
    rotA -= 1.5f * elapsedSeconds;
  }
  if(keys.IsKeyDown(Keys::K_RIGHT))
  {
    rotA += 1.5f * elapsedSeconds;
  }

  if(keys.IsKeyDown(Keys::K_W))
  {
    y += 9.5f * elapsedSeconds;
  }
  if(keys.IsKeyDown(Keys::K_S))
  {
    y -= 9.5f * elapsedSeconds;
  }

  if(gamePad.IsButtonDown(ControllerButtons::DPAD_UP))
  {
    y = 9.5f * elapsedSeconds;
  }
  if(gamePad.IsButtonDown(ControllerButtons::DPAD_DOWN))
  {
    y = -9.5f * elapsedSeconds;
  }

  m_pMeshNode->Translate(x, y, z); 
  m_pMeshNode->Rotate(rotA, 0, 0);

  if(keys.IsKeyDown(Keys::K_ESCAPE))// || gamePad.IsButtonDown(ControllerButtons::BACK))
  {
    Exit();
  }
}

void TestApp::OnRender()
{
  GetGraphicsProvider()->Clear(Color4f(0.0f, 0.15f, 0.0f, 0.0f));
  m_pScene->Render();
}

void TestApp::OnDeviceReset()
{
  BaseGame::OnDeviceReset();

  if(m_pScene)
  {
    m_pScene->OnDeviceReset();
  }
}

void TestApp::OnDeviceLost()
{
  BaseGame::OnDeviceLost();

  if(m_pScene)
  {
    m_pScene->OnDeviceLost();
  }
}
