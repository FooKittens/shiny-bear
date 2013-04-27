#ifndef TESTAPP_H
#define TESTAPP_H

#include <base\BaseGame.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>

namespace shinybear
{
  class SceneManager;
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
};

#endif