#ifndef TESTAPP_H
#define TESTAPP_H

#include <base\BaseGame.h>
#include <scene\LightNode.h>
#include <graphics\Light.h>
#include <graphics\VertexFormats.h>
#include <util\input\InputManager.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>


namespace shinybear
{
  class SceneManager; class SceneView;
  class Mesh; class MeshNode; struct BlockMaterial;
  class Camera;
}

class TerrainGenerator;

class TestApp : public shinybear::BaseGame
{
public:
  TestApp();
  ~TestApp();

protected:
  bool OnInitialize();

  void OnUpdate(double elapsedSeconds);
  void OnRender();

  shinybear::MeshNode *CreateMeshNode(shinybear::BlockColor mat);

  void OnDeviceLost();
  void OnDeviceReset();
private:

  void CreateCubes();
  void CreateLights();
  void CreateRandomLights();

  // Terrain generator for creating clusters.
  TerrainGenerator *m_pGenerator;

  shinybear::Camera *m_pCamera;

  shinybear::SceneManager *m_pScene;
  shinybear::MeshNode *m_pPlayerNode;
  shinybear::MeshNode *m_pSunCubeNode;
  shinybear::MeshNode *m_pSunAxisNode;

  // Some random lights.
  shinybear::Light m_ambientLight;
  shinybear::Light m_sunLight;
  shinybear::Light m_playerLight;
  shinybear::Light m_randomLight;

  shinybear::LightNode *m_pAmbientLightNode;
  shinybear::LightNode *m_pSunLightNode;
  shinybear::LightNode *m_pPlayerLightNode;
  shinybear::LightNode *m_pRandomLightNode;

  // Materials for testing.
  shinybear::BlockColor m_grassMaterial;
  shinybear::BlockColor m_metalMaterial;
  shinybear::BlockColor m_shinyMaterial;

  shinybear::KeyboardState m_newKeys;
  shinybear::KeyboardState m_oldKeys;
  shinybear::MouseState m_newMouse;
  shinybear::GamePadState m_gamePadState;

  std::vector<shinybear::MeshNode *> m_meshes;
};

#endif