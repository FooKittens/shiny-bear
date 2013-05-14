#ifndef TESTAPP_H
#define TESTAPP_H

#include "TestDrawable.h"

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
  class Camera; class WorldManager;
}

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
  TestDrawable *m_pDrawable;

  // Terrain generator for creating clusters.
  shinybear::WorldManager *m_pWorldManager;

  shinybear::Camera *m_pCamera;

  // Materials for testing.
  shinybear::BlockColor m_grassMaterial;
  shinybear::BlockColor m_metalMaterial;
  shinybear::BlockColor m_shinyMaterial;

  shinybear::KeyboardState m_newKeys;
  shinybear::KeyboardState m_oldKeys;
  shinybear::MouseState m_newMouse;
  shinybear::GamePadState m_gamePadState;
};

#endif