#ifndef DXWINDOW_H
#define DXWINDOW_H
#include <Windows.h>

namespace framework {

class DXWindow  {
public:
	DXWindow(HINSTANCE app_instance, const SIZE &size);
	virtual ~DXWindow();

  // Updates the window, processing all messages on the queue.
  virtual void HandleMessages();

  inline virtual SIZE GetSize() { return m_size; }
  virtual void SetSize(SIZE size);
  inline virtual HWND GetWindowHandle() { return m_hwnd; }


  inline virtual void ShowWindow() { ::ShowWindow(m_hwnd, TRUE); }
  inline virtual void HideWindow() { ::ShowWindow(m_hwnd, FALSE); }
  inline virtual void SetTitle(const char *title) {
    SetWindowText(m_hwnd, title);
  }

protected:
  LRESULT virtual CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

private:
  // Used to delegate calls to the instance method.
	static LRESULT CALLBACK StaticWinProc(HWND, UINT, WPARAM, LPARAM);
  
  static const char *kWndClassName;
  HWND m_hwnd;
	WNDCLASS m_wndClass;
  SIZE m_size;
  HINSTANCE m_appInstance;
};


} // namespace framework
#endif