
#define DEBUG
#define D3D_DEBUG_INFO

#include "Framework\Framework.h"
#include "TestApp.h"
#include <Windows.h>
#include <crtdbg.h>


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev,
  char *cmdLine, int cmdshow) {     
  
  // Enable memory leak checks
#if defined(DEBUG) | defined (_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF |
      _CRTDBG_LEAK_CHECK_DF );
#endif               

  SIZE sz = { 800, 600 };
  TestApp ta(hInst, sz);
  ta.Initialize();
  ta.Run();
}