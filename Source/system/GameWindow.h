#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <Windows.h>
#include <string>

namespace shinybear {

typedef unsigned int UINT;

struct Size {
  UINT width;
  UINT height;
};

class GameWindow {
public:
  // Creates the singleton window.
  static void Create(const Size &size);

  // Handle Win32 messages.
  void HandleMessages();

  static const Size& GetSize();
  void SetSize(const Size &size);

  // Retrieves a handle to the window for lowlevel access.
  static HWND GetWindowHandle();
  
  static void SetTitle(const std::string &title);
  static void SetTitle(const char *title);
  
  static void IsVisible();
  static void Show();
  static void Hide();

  // Frees resources used by window.
  static void Cleanup();

protected:
  // Window procedure for overrriding.
  LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

private:
  // GameWindows should not be copied or assigned.
  GameWindow(const GameWindow&);
  const GameWindow& operator=(const GameWindow&);

  // Since this is a singleton, nobody should create or destroy
  // windows except the class itself.
  GameWindow(const Size &size);
  virtual ~GameWindow();

  // Static winproc for delegating to instance method.
  static LRESULT CALLBACK StaticWinProc(HWND, UINT, WPARAM, LPARAM);

  // Used when registring the class with Win32.
  static const char *s_className;

  // Singleton instance.
  static GameWindow* s_singleton;
  
  
  // Member fields.
  Size m_size;
  HWND m_hwnd;
  bool m_isVisible;
};

// --------- Inlines ----------
inline void GameWindow::SetSize(const Size &size) {
  m_size = size;
  SetWindowPos(s_singleton->m_hwnd, 0, 0, 0, size.width, size.height, SWP_NOMOVE);
}

inline void GameWindow::SetTitle(const std::string &title) {
  SetTitle(title.c_str());
}

inline void GameWindow::SetTitle(const char *title) {
  SetWindowText(s_singleton->m_hwnd, title);
}

inline HWND GameWindow::GetWindowHandle() {
  return s_singleton->m_hwnd;
}




} // namespace shinybear


#endif