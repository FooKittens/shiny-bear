#include "TestApp.h"
#include <base\system\GraphicsProvider.h>

using namespace shinybear;

TestApp::TestApp() 
{

}

TestApp::~TestApp()
{
  
}

bool TestApp::OnInitialize() 
{

  return true;
}


void TestApp::OnUpdate(double elapsedSeconds) 
{
  BaseGame::OnUpdate(elapsedSeconds);

}

void TestApp::OnRender()
{
  GetGraphicsProvider()->Clear(Color4f(0, 0, 0, 0));
}


