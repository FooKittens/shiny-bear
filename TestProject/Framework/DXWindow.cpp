
#include "DXWindow.h"
#include <cassert>


namespace framework {

const char *DXWindow::kWndClassName = "DX_WINCLASS";

DXWindow::DXWindow(HINSTANCE app_instance, const SIZE &size) {
	m_size = size;
  m_hwnd = 0;
  m_appInstance = app_instance;

  // Configure WindowClass
  memset(&m_wndClass, 0, sizeof(WNDCLASS));
  m_wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  m_wndClass.hInstance = m_appInstance;
  m_wndClass.lpfnWndProc = &DXWindow::StaticWinProc;
  m_wndClass.lpszClassName = kWndClassName;

  RegisterClass(&m_wndClass);

  m_hwnd = CreateWindow(
    kWndClassName,
    "DXWindow",
    WS_OVERLAPPED | WS_SYSMENU,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    m_size.cx,
    m_size.cy,
    NULL,
    NULL,
    m_appInstance,
    this // Pass in a pointer to this object for dispatching in static wndproc.
  );

  // Crash on failing to create window.
  assert(m_hwnd && "Failed to create window!");

}

DXWindow::~DXWindow() {

}

void DXWindow::HandleMessages() {
  MSG msg = { 0 };
  while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
  }
}

// Changes the windows size programmatically, normal resizing is blocked.
void DXWindow::SetSize(SIZE size) {
  SetWindowPos(m_hwnd, 0, 0, 0, size.cx, size.cy, SWP_NOMOVE);
}

LRESULT CALLBACK DXWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

  switch(msg)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}

// Used to delegate calls to the instance method for the winproc.
LRESULT CALLBACK DXWindow::StaticWinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if(msg == WM_CREATE) {
    DXWindow *p_window = (DXWindow*)((LPCREATESTRUCT)lparam)->lpCreateParams;

    assert(p_window && "Failed to retrieve DXWindow pointer from lparam!");

    SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<LONG>(p_window));

    // Make sure the target window recieves the WM_CREATE message aswell.
    p_window->WindowProc(hwnd, msg, wparam, lparam);
  }

  DXWindow *p_window = (DXWindow*)GetWindowLong(hwnd, GWL_USERDATA);
    
  if(p_window) {
    // Make sure a window exists.
    assert(p_window && "Failed to retrieve DXWindowPointer from windowlong!");

    return p_window->WindowProc(hwnd, msg, wparam, lparam);
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

} // namespace Framework
