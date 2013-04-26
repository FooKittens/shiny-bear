#include "base\system\GameWindow.h"
#include "util\SBUtil.h"
#include "util\input\InputManager.h"
#include <cassert>
#include <WtsApi32.h>

using std::string;

namespace shinybear
{

// name used by windows to identify the class internally.
const char *GameWindow::s_className = "GAMEWINDOW_CLASS";

GameWindow::GameWindow(const Size &size)
{
  m_size = size;
  m_isVisible = false;

  // Set up window class parameters.
  WNDCLASS wclass;
  memset(&wclass, 0, sizeof(WNDCLASS));
  wclass.hInstance = GetModuleHandle(NULL);
  wclass.lpfnWndProc = &GameWindow::StaticWinProc;
  wclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wclass.lpszClassName = s_className;

  // Register the class for usage.
  RegisterClass(&wclass);

  // Create a window using our window class.
  m_hwnd = CreateWindow(
    s_className,
    "Default Window Title",
    WS_OVERLAPPED | WS_SYSMENU,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    m_size.width,
    m_size.height,
    NULL,
    NULL,
    GetModuleHandle(NULL),
    this
  );
}

GameWindow::~GameWindow()
{

}

void GameWindow::Show()
{
  ShowWindow(m_hwnd, TRUE);
}

void GameWindow::Hide()
{
  ShowWindow(m_hwnd, FALSE);
}

void GameWindow::HandleMessages()
{
  MSG msg = { 0 };

  while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

LRESULT GameWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  // OutputDbgFormat("WindowsMessage: [%i]", msg);
  
  switch(msg)
  {
  case WM_ACTIVATEAPP:

    return DefWindowProc(hwnd, msg, wparam, lparam);
  case WM_INPUT:
    if(wparam == RIM_INPUT)
    {
      HRAWINPUT hInput = reinterpret_cast<HRAWINPUT>(lparam);
      InputManager::HandleInput(hInput);
    }
    
    return DefWindowProc(hwnd, msg, wparam, lparam);
  case WM_DESTROY:

    return 0;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

// Simply forwards the window proc to a member version.
LRESULT CALLBACK GameWindow::StaticWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  
  // Store the pointer to the window in the windows long on wm_create
  if(msg == WM_CREATE)
  {
    // Retrieve pointer from lparam
    GameWindow *pWindow = (GameWindow*)((LPCREATESTRUCT)lparam)->lpCreateParams;

    assert(pWindow && "Did not find window in lparam!");

    // Store the window in the variable maintained by windows.
    SetWindowLong(hwnd, GWL_USERDATA, (LONG)pWindow);
  }

  // Retrieve window from windowlong.
  GameWindow *pWindow = (GameWindow*)GetWindowLong(hwnd, GWL_USERDATA);

  if(pWindow)
  {
    return pWindow->WindowProc(hwnd, msg, wparam, lparam);
  }
  
  return DefWindowProc(hwnd, msg, wparam, lparam);
}


} // namespace shinybear