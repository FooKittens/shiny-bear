#ifndef TESTAPP_H
#define TESTAPP_H

#include <base\BaseGame.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>

namespace shinybear
{
  class SceneManager; class SceneView;
  class Mesh; class MeshNode;
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

  //void OnDeviceLost();
  //void OnDeviceReset();

private:
  shinybear::SceneManager *m_pScene;
  shinybear::MeshNode *m_pMeshNode;
  shinybear::MeshNode *m_pOtherNode;
  shinybear::MeshNode *m_pThirdNode;
};

#endif