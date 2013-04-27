#include "TestApp.h"
#include <base\system\GraphicsProvider.h>
#include <scene\SceneManager.h>

using namespace shinybear;

TestApp::TestApp() 
{

}

TestApp::~TestApp()
{
  delete m_pScene;
}

bool TestApp::OnInitialize() 
{
  m_pScene = new SceneManager(GetGraphicsProvider());
  return true;
}


void TestApp::OnUpdate(double elapsedSeconds) 
{
  BaseGame::OnUpdate(elapsedSeconds);
  m_pScene->Update(elapsedSeconds);
}

void TestApp::OnRender()
{
  GetGraphicsProvider()->Clear(Color4f(0, 0, 0, 0));
  m_pScene->Render();
}


