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

#include "TerrainGenerator.h"
#include "RandomMover.h"

using namespace shinybear;


TestApp::TestApp()
  :m_gamePadState(GamePadIndex::ONE)
{
  m_pScene            = nullptr;
  m_pPlayerNode       = nullptr;
  m_pSunCubeNode      = nullptr;
  m_pSunAxisNode      = nullptr;
  m_pAmbientLightNode = nullptr;
  m_pSunLightNode     = nullptr;
  m_pRandomLightNode  = nullptr;
  m_pGenerator        = nullptr;
}

TestApp::~TestApp()
{
  //for(int i = 0; i < m_meshes.size(); ++i)
  //{
  //  delete m_meshes[i];
  //}

  //m_meshes.clear();

  delete m_pGenerator;
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

bool TestApp::OnInitialize() 
{
  m_pScene = DBG_NEW SceneManager(GetGraphicsProvider());
  m_pGenerator = DBG_NEW TerrainGenerator(GetGraphicsProvider(), m_pScene->GetRoot());

  // Set up material parameters.
  #pragma region MaterialConfig
  m_grassMaterial.diffuse   = 0x00036804;
  m_grassMaterial.specular  = 0x03FFFFFF;
  m_metalMaterial.diffuse   = 0x00423838;
  m_metalMaterial.specular  = 0x43FFFFFF;
  m_shinyMaterial.diffuse   = 0x00AAAAAA;
  m_shinyMaterial.specular  = 0xFFFFFFFF;
  #pragma endregion

  CreateLights();
  CreateCubes();

  // Attach ambient light to the scene.
  m_pScene->GetRoot()->Attach(m_pAmbientLightNode);

  // Translate the sun straight up vertically to give it a good axis.
  m_pSunCubeNode->Translate(-3.0f, 10.0f, 5.0f);
 
  // Attach sun light to the suns cube.
  m_pSunCubeNode->Attach(m_pSunLightNode);

  // Attach the sun and its cube to the axis around which they will spin.
  m_pSunAxisNode->Attach(m_pSunCubeNode);

  // Create a camera that follows the player.
  CameraNode *pCam = new CameraNode(GetWindow(), m_pPlayerNode);
  m_pScene->SetCamera(pCam);

  // Attach player light node to the player.
  m_pPlayerNode->Attach(m_pPlayerLightNode);

  // Attach the sun axis along with its cube and sun light.
  m_pScene->GetRoot()->Attach(m_pSunAxisNode);

  // Attach the player to the root node.
  m_pScene->GetRoot()->Attach(m_pPlayerNode);

  // Generate an 8x8 world.
  m_pGenerator->Generate(8, 8);

  // Creates a set amount of random lights flying about the scene.
  CreateRandomLights();

  // Initialization successful.
  return true;
}

void TestApp::CreateLights()
{
  // Create an ambient light for the scene.
  m_ambientLight = Light::CreateAmbientLight(D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f));
  m_pAmbientLightNode = DBG_NEW LightNode(m_ambientLight);

  // Create light to act as "sun"
  m_sunLight = Light::CreateDirectionalLight(D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f),
    Vector3(0, -1, 0));
  m_pSunLightNode = DBG_NEW LightNode(m_sunLight);

  m_playerLight = Light::CreatePointLight(D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f),
    Vector3(0, 0, 0), Vector3(0.35f, 0.025f, 0.01f));

  m_pPlayerLightNode = DBG_NEW LightNode(m_playerLight);
}

void TestApp::CreateRandomLights()
{
  for(int i = 0; i < 50; ++i)
  {
    RandomMover *pMover = DBG_NEW RandomMover(Vector3(0, 2.0f, 0), 75.0f);
    float r = (rand() % 200 + 55) / 255.0f;
    float g = (rand() % 200 + 55) / 255.0f;
    float b = (rand() % 200 + 55) / 255.0f;

    Light light = Light::CreatePointLight(
      D3DXCOLOR(r, g, b, 1.0f), // Color
      Vector3(0,0,0), // position - Not used atm.
      Vector3(0.35f, 0.025f, 0.01f) // Attuneation.
    );

    LightNode *pLightNode = DBG_NEW LightNode(light);
    pMover->Attach(pLightNode);
    m_pScene->GetRoot()->Attach(pMover);
  }
}

void TestApp::CreateCubes()
{
  m_pPlayerNode = CreateMeshNode(&m_metalMaterial);
  m_pSunCubeNode = CreateMeshNode(&m_metalMaterial);
  m_pSunAxisNode = CreateMeshNode(&m_metalMaterial);
}

const float kPlayerSpeed = 9.5f;
const float kPlayerAngSpeed = 2.5f;

void TestApp::OnUpdate(double elapsedSeconds) 
{
  BaseGame::OnUpdate(elapsedSeconds);
  m_pScene->Update(elapsedSeconds);

  InputManager::GetControllerState(&m_gamePadState);
  InputManager::GetKeyboardState(&m_newKeys);
  InputManager::GetMouseState(&m_newMouse);

  float rotA = 0.0f;
  float rotB = 0.0f;
  float rotC = 0.0f;
  float z = 0.0f;
  float x = 0.0f;
  float y = 0.0f;

  rotA = kPlayerAngSpeed * (float)elapsedSeconds * m_gamePadState.rightThumbstick.x;
  rotA += kPlayerAngSpeed * (float)elapsedSeconds * m_newMouse.GetPositionalChange().x;
  //rotB += kPlayerAngSpeed * (float)elapsedSeconds * m_newMouse.GetPositionalChange().y;
  z -= kPlayerSpeed * (float)elapsedSeconds * m_gamePadState.leftTrigger;
  z += kPlayerSpeed * (float)elapsedSeconds * m_gamePadState.rightTrigger;

  m_gamePadState.Vibrate(m_gamePadState.leftTrigger,
    m_gamePadState.rightTrigger);

  if(m_newKeys.IsKeyDown(Keys::K_ADD))
  {
    rotB = kPlayerAngSpeed * (float)elapsedSeconds;
  }
  if(m_newKeys.IsKeyDown(Keys::K_SUBTRACT))
  {
    rotB = -kPlayerAngSpeed * (float)elapsedSeconds;
  }

  if(m_newKeys.IsKeyDown(Keys::K_UP))
  {
    z = kPlayerSpeed * (float)elapsedSeconds;
  }
  if(m_newKeys.IsKeyDown(Keys::K_DOWN))
  {
    z = -kPlayerSpeed * (float)elapsedSeconds;
  }

  if(m_newKeys.IsKeyDown(Keys::K_LEFT))
  {
    rotA = -kPlayerAngSpeed * (float)elapsedSeconds;
  }
  if(m_newKeys.IsKeyDown(Keys::K_RIGHT))
  {
    rotA = kPlayerAngSpeed * (float)elapsedSeconds;
  }

  if(m_newKeys.IsKeyDown(Keys::K_W))
  {
    y = kPlayerSpeed * (float)elapsedSeconds;
  }
  if(m_newKeys.IsKeyDown(Keys::K_S))
  {
    y = -kPlayerSpeed * (float)elapsedSeconds;
  }

  if(m_gamePadState.IsButtonDown(ControllerButtons::DPAD_UP))
  {
    y = kPlayerSpeed * (float)elapsedSeconds;
  }
  if(m_gamePadState.IsButtonDown(ControllerButtons::DPAD_DOWN))
  {
    y = -kPlayerSpeed * (float)elapsedSeconds;
  }

  m_pPlayerNode->Translate(x, y, z); 
  m_pPlayerNode->Rotate(rotA, 0, 0);
  m_pSunAxisNode->Rotate(0, rotB, 0);
  if(m_newKeys.IsKeyDown(Keys::K_ESCAPE) || m_gamePadState.IsButtonDown(ControllerButtons::BACK))
  {
    Exit();
  }

  m_oldKeys = m_newKeys;
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
