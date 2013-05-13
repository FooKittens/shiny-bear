#include "TestApp.h"
#include <base\BaseGame.h>
#include <Windows.h>
#include <crtdbg.h>
#include <util\SBUtil.h>


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev,
  char *cmdLine, int cmdshow)
{
  // Enable memory leak checks
#if defined(DEBUG) | defined (_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF |
      _CRTDBG_LEAK_CHECK_DF );
#endif               

  shinybear::BaseGame *pGame = new TestApp();
  pGame->Run();

  delete pGame;
  return 0;
}