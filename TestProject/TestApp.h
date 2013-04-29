#ifndef TESTAPP_H
#define TESTAPP_H

#include <base\BaseGame.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>


namespace shinybear
{
  class SceneManager; class SceneView;
  class Mesh; class MeshNode; struct BlockMaterial;
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

  shinybear::MeshNode *CreateMeshNode(shinybear::BlockMaterial *mat);

  //void OnDeviceLost();
  //void OnDeviceReset();

private:
  shinybear::SceneManager *m_pScene;
  shinybear::MeshNode *m_pMeshNode;
  shinybear::MeshNode *m_pOtherNode;
  shinybear::MeshNode *m_pThirdNode;
  std::vector<shinybear::MeshNode *> m_meshes;
};

#endif