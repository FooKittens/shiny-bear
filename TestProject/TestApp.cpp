#include "TestApp.h"
#include <util\input\InputManager.h>
#include <base\system\GameWindow.h>

#include <Windows.h>


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
  KeyboardState keys = KeyboardState();
  keys = InputManager::GetKeyboardState();
  if(keys.IsKeyDown(Keyboard::K_ESCAPE))
  {
    exit(0);
  }
}


