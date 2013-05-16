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
#include <graphics\lighting\Lighting.h>
#include <graphics\render\IDrawable.h>
#include <world\WorldManager.h>
#include <world\Block.h>
#include <world\Cluster.h>
#include <util\SBUtil.h>


#include "TerrainGenerator.h"
#include "RandomMover.h"
#include "TestDrawable.h"

using namespace shinybear;


TestApp::TestApp()
  :m_gamePadState(GamePadIndex::ONE)
{
  m_pWorldManager     = nullptr;

  m_pCamera = nullptr;
  m_pDrawable = nullptr;
  m_pRenderer = nullptr;
}

TestApp::~TestApp()
{
  auto it = m_lights.begin();
  auto end = m_lights.end();

  while(it != end)
  {
    delete *it++;
  }
  m_lights.clear();

  //for(auto it = m_drawables.begin(); it != m_drawables.end(); ++it)
  //{
  //  delete *it;
  //}

  m_drawables.clear();

  

  delete m_pRenderer;
  delete m_pWorldManager;
  delete m_pCamera;
  delete m_pDrawable;
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
  // Set up material parameters.
  #pragma region MaterialConfig
  m_grassMaterial = 0x03036804;
  m_metalMaterial = 0x43423838;
  m_shinyMaterial = 0x44AAAAAA;
  #pragma endregion

  m_pRenderer = DBG_NEW Renderer(GetGraphicsProvider());
  m_pRenderer->Initialize();

  m_pDrawable = DBG_NEW TestDrawable();
  m_drawables.push_back(m_pDrawable);
  m_pRenderer->AddDrawable(m_pDrawable);

  // Create a camera.
  Size wSize = GetWindow()->GetSize();
  float aspect = (float)wSize.width / (float)wSize.height;
  m_pCamera = DBG_NEW Camera(aspect, 60.0f, 1.0f, 100.0f);
  m_pCamera->SetDebugMode(true);

  Mesh *pMesh = DBG_NEW Mesh(GetGraphicsProvider());
  Block b(0);
  b.SetMaterial(m_shinyMaterial);
  CreateCube(0, 0, 0, b, pMesh, 0);
  pMesh->UpdateBuffers();
  m_pDrawable->SetMesh(pMesh);

  m_pRenderer->SetCamera(m_pCamera);

  
  //m_pRenderer->AddLight(DBG_NEW BaseLight(D3DXCOLOR(0.1f, 0.1f, 0.13f, 1.0f)));
  //m_pRenderer->AddLight(DBG_NEW DirectionalLight(
  //  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), Vector3(1.0f, -0.7f, -1.0f))
  //);

  //PointLight *pLight = DBG_NEW PointLight(
  //  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 2.0f, 0.0f), 2.0f, 50.0f);
  //m_pRenderer->AddLight(pLight);

  //m_pDrawable->m_world = Mat4x4::CreateScale(1.0f) *
  //  Mat4x4::CreateTranslation(pLight->m_position + Vector3::kUnitX);

  m_pWorldManager = DBG_NEW WorldManager(GetGraphicsProvider(), m_pRenderer);
  m_pWorldManager->Generate(4, 4);

  //CreateLights();

  // Initialization successful.
  return true;
}

void TestApp::CreateLights()
{
  D3DXCOLOR color;
  for(int i = 0; i < 25; ++i)
  {
    float r = (rand() % 100 + 155) / 255.0f;
    float g = (rand() % 100 + 155) / 255.0f;
    float b = (rand() % 100 + 155) / 255.0f;


    PointLight *pLight = DBG_NEW PointLight(
      D3DXCOLOR(r, g, b, 1.0f), Vector3::kZero, 0.0f, 35.0f);

    m_lights.push_back(DBG_NEW RandomMover(pLight, Vector3::kUnitY * 5.0f, 50.0f));
    m_pRenderer->AddLight(pLight);
  }
}

const float kPlayerSpeed = 9.5f;
const float kPlayerAngSpeed = 2.5f;

void TestApp::OnUpdate(double elapsedSeconds) 
{
  BaseGame::OnUpdate(elapsedSeconds);
  m_pCamera->Update(elapsedSeconds);

  size_t sz = m_lights.size();
  for(int i = 0; i < sz; ++i)
  {
    m_lights[i]->Update(elapsedSeconds);
  }

  InputManager::GetControllerState(&m_gamePadState);
  InputManager::GetKeyboardState(&m_newKeys);
  InputManager::GetMouseState(&m_newMouse);

  if(m_newKeys.IsKeyDown(Keys::K_ESCAPE) || m_gamePadState.IsButtonDown(ControllerButtons::BACK))
  {
    Exit();
  }

  m_oldKeys = m_newKeys;
}

void TestApp::OnRender()
{
  // Clear backbuffer to black.
  m_pRenderer->Clear(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
  m_pRenderer->BeginScene();

  m_pRenderer->RenderScene();

  RenderDiagnostics();

  m_pRenderer->EndScene();
  m_pRenderer->Present();  
}

void TestApp::OnDeviceReset()
{
  BaseGame::OnDeviceReset();
}

void TestApp::OnDeviceLost()
{
  BaseGame::OnDeviceLost();
}
