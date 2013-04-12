#include "system\GameWindow.h"

using std::string;

namespace shinybear {

// name used by windows to identify the class internally.
const char *GameWindow::s_className = "GAMEWINDOW_CLASS";

GameWindow::GameWindow(const Size &size) {

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
    size.width,
    size.height,
    NULL,
    NULL,
    GetModuleHandle(NULL),
    this
  );
  
}

GameWindow::~GameWindow() {

}

// Frees resources used by window, simply triggers the destructor of our singleton.
void GameWindow::Cleanup() {
  delete s_singleton;
}

void GameWindow::Create(const Size &size) {
  s_singleton = new GameWindow(size);
}

void GameWindow::Show() {
  ShowWindow(s_singleton->m_hwnd, TRUE);
}

void GameWindow::Hide() {
  ShowWindow(s_singleton->m_hwnd, FALSE);
}

void GameWindow::HandleMessages() {
  MSG msg = { 0 };

  while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

LRESULT CALLBACK GameWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {
  case WM_DESTROY:
    // User closed window.
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

// Simply forwards the window proc to a member version.
LRESULT CALLBACK GameWindow::StaticWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return s_singleton->WindowProc(hwnd, msg, wparam, lparam);
}


} // namespace shinybear