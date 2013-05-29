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
#include <scene\camera\Camera.h>
#include <graphics\Mesh.h>
#include <graphics\Light.h>
#include <world\Block.h>
#include <world\Cluster.h>
#include <util\SBUtil.h>
#include <resource\ResourceManager.h>
#include <sound\WaveFile.h>
#include <sound\OggFile.h>

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
  BlockColor mat = block.GetMaterial();

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

MeshNode *TestApp::CreateMeshNode(BlockColor mat)
{
  static Block block;
  block.SetMaterial(mat);

  Mesh *pMesh = DBG_NEW Mesh(GetGraphicsProvider());
  MeshNode *pNode = DBG_NEW MeshNode(pMesh);

  pMesh->BeginMesh();
  CreateCube(0, 0, 0, block, pMesh, 0);
  pMesh->EndMesh();
  return pNode;
}

bool TestApp::OnInitialize()
{
  // Sounds!
  wchar_t *pStrBuffer;
  size_t strSize = GetAbsolutePath(L"res\\sounds\\Wololo.wav", &pStrBuffer);
  if(FileExists(pStrBuffer, strSize))
  {
    WaveFile *testSound = WaveFile::LoadFromFile(pStrBuffer, GetSoundProvider());
    ResourceManager::RegisterResource(testSound, "Wololo");
  }
  delete[] pStrBuffer;

  strSize = GetAbsolutePath(L"res\\sounds\\Wololo Blues.ogg", &pStrBuffer);
  if(FileExists(pStrBuffer, strSize))
  {
    OggFile *oggTest = OggFile::LoadFromFile(pStrBuffer, GetSoundProvider());
    ResourceManager::RegisterResource(oggTest, "Blues");
  }
  delete[] pStrBuffer;

  strSize = GetAbsolutePath(L"res\\sounds\\Roggan.ogg", &pStrBuffer);
  if(FileExists(pStrBuffer, strSize))
  {
    OggFile *oggTest = OggFile::LoadFromFile(pStrBuffer, GetSoundProvider());
    ResourceManager::RegisterResource(oggTest, "Roggan");
  }
  delete[] pStrBuffer;

  // Graphics!
  m_pScene = DBG_NEW SceneManager(GetGraphicsProvider());
  m_pGenerator = DBG_NEW TerrainGenerator(GetGraphicsProvider(), m_pScene->GetRoot());

  // Set up material parameters.
  #pragma region MaterialConfig
  m_grassMaterial = 0x03036804;
  m_metalMaterial = 0x43423838;
  m_shinyMaterial = 0xFFAAAAAA;
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

  // Create a camera.
  Size wSize = GetWindow()->GetSize();
  float aspect = (float)wSize.width / (float)wSize.height;
  m_pCamera = new Camera(aspect, 60.0f, 1.0f, 100.0f);
  m_pCamera->SetDebugMode(true);
  m_pScene->SetCamera(m_pCamera);

  // Attach player light node to the player.
  //m_pPlayerNode->Attach(m_pPlayerLightNode);

  // Attach the sun axis along with its cube and sun light.
  m_pScene->GetRoot()->Attach(m_pSunAxisNode);

  // Attach the player to the root node.
  m_pScene->GetRoot()->Attach(m_pPlayerNode);

  // Generate an 8x8 world.
  m_pGenerator->Generate(1, 1);

  // Creates a set amount of random lights flying about the scene.
  CreateRandomLights();

  // Initialization successful.
  return true;
}

void TestApp::CreateLights()
{
  // Create an ambient light for the scene.
  m_ambientLight = Light::CreateAmbientLight(D3DXCOLOR(0.15f, 0.15f, 0.19f, 1.0f));
  m_pAmbientLightNode = DBG_NEW LightNode(m_ambientLight);

  // Create light to act as "sun"
  m_sunLight = Light::CreateDirectionalLight(D3DXCOLOR(0.30f, 0.35f, 0.30f, 1.0f),
    Vector3(0, -1, 0));
  m_pSunLightNode = DBG_NEW LightNode(m_sunLight);

  m_playerLight = Light::CreatePointLight(D3DXCOLOR(0.95f, 0.95f, 0.95f, 1.0f),
    Vector3(0, 0, 0), 0.50f, 100.0f);

  m_pPlayerLightNode = DBG_NEW LightNode(m_playerLight);
  m_pPlayerLightNode->Translate(0, 2.0f, 0);
}

void TestApp::CreateRandomLights()
{
  for(int i = 0; i < 25; ++i)
  {
    RandomMover *pMover = DBG_NEW RandomMover(Vector3(0, -7.5f, 0), 70.0f);
    float r = (rand() % 100 + 155) / 255.0f;
    float g = (rand() % 100 + 155) / 255.0f;
    float b = (rand() % 100 + 155) / 255.0f;

    Light light = Light::CreatePointLight(
      D3DXCOLOR(r, g, b, 1.0f), // Color
      Vector3(0,0,0), // position - Not used atm.
      0.3f, // Dim factor
      rand() % 50 + 25 // Attenuation.
    );

    LightNode *pLightNode = DBG_NEW LightNode(light);
    pMover->Attach(pLightNode);
    m_pScene->GetRoot()->Attach(pMover);
  }
}

void TestApp::CreateCubes()
{
  m_pPlayerNode = CreateMeshNode(m_metalMaterial);
  m_pSunCubeNode = CreateMeshNode(m_metalMaterial);
  m_pSunAxisNode = CreateMeshNode(m_metalMaterial);

  //m_pPlayerNode->Rotate(D3DX_PI / 2.0f, 0, 0);
}

const float kPlayerSpeed = 9.5f;
const float kPlayerAngSpeed = 2.5f;
unsigned long frequency = 0;

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
  rotB += kPlayerAngSpeed * (float)elapsedSeconds * m_newMouse.GetPositionalChange().y;
  z -= kPlayerSpeed * (float)elapsedSeconds * m_gamePadState.leftTrigger;
  z += kPlayerSpeed * (float)elapsedSeconds * m_gamePadState.rightTrigger;

  frequency = 44100 + m_gamePadState.leftThumbstick.y * 11025;
  ResourceManager::GetResource<ISoundResource>("Wololo")->SetFreq(frequency);
  ResourceManager::GetResource<ISoundResource>("Blues")->SetFreq(frequency);
  ResourceManager::GetResource<ISoundResource>("Roggan")->SetFreq(frequency);

  m_gamePadState.Vibrate(m_gamePadState.leftTrigger,
    m_gamePadState.rightTrigger);

  if(m_gamePadState.IsButtonDown(ControllerButtons::GAMEPAD_A) && !m_oldKeys.IsKeyDown(Keys::K_NUMPAD0))
  {
    ResourceManager::GetResource<ISoundResource>("Wololo")->Play();
  }

  if(m_newKeys.IsKeyDown(Keys::K_NUMPAD1) && !m_oldKeys.IsKeyDown(Keys::K_NUMPAD1))
  {
    ResourceManager::GetResource<ISoundResource>("Blues")->Play();
  }

  if(m_newKeys.IsKeyDown(Keys::K_NUMPAD2) && !m_oldKeys.IsKeyDown(Keys::K_NUMPAD2))
  {
    ResourceManager::GetResource<ISoundResource>("Roggan")->Play();
  }

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

  //if(m_pCamera->IsFreeCam())
  //{
  //  m_pCamera->Translate(x, y, z);
  //  m_pCamera->RotateY(rotA);
  //}
  //else
  //{
  //  m_pPlayerNode->Translate(x, y, z);
  //  m_pPlayerNode->RotateY(rotA);
  //}

  if(m_newKeys.IsKeyDown(Keys::K_SPACEBAR) && !m_oldKeys.IsKeyDown(Keys::K_SPACEBAR))
  {
    //bool isFree = m_pCamera->IsFreeCam();
    //if(isFree)
    //{
    //   m_pCamera->LoadIdentity();
    //}
    //m_pCamera->SetFreeCam(!isFree);
  }

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
