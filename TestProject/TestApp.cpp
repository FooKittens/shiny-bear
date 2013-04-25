#include "TestApp.h"
#include <base\system\GameWindow.h>
#include <view\RenderView.h>
#include <Windows.h>


using namespace shinybear;

TestApp::TestApp() 
{

}

TestApp::~TestApp()
{
  delete m_pPlayerView;
}

bool TestApp::OnInitialize() 
{

  m_pPlayerView = new PlayerView(GetGraphicsProvider());

  AttachView(m_pPlayerView);
  return true;
}


void TestApp::OnUpdate(double elapsedSeconds) 
{

}


