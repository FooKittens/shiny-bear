#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "util\Typedefs.h"
#include <Windows.h>
#include <string>

namespace shinybear { class BaseGame; }

namespace shinybear
{

struct Size
{
  UINT width;
  UINT height;
};

class GameWindow
{
public:
  GameWindow(BaseGame *pGame, const Size &size);
  ~GameWindow();

  // Handle Win32 messages.
  void HandleMessages();

  const Size &GetSize() const;
  void SetSize(const Size &size);

  // Retrieves a handle to the window for lowlevel access.
  HWND GetWindowHandle() const;
  
  void SetTitle(const char *title);
  
  bool IsVisible();
  void Show();
  void Hide();
protected:

private:
  // GameWindows should not be copied or assigned.
  GameWindow(const GameWindow&);
  const GameWindow& operator=(const GameWindow&);

  // Instance
  LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);
  
  // Static winproc for delegating to instance method.
  static LRESULT CALLBACK StaticWinProc(HWND, UINT, WPARAM, LPARAM);

  // Used when registring the class with Win32.
  static const char *s_className; 
  
  // Member fields.
  Size m_size;
  HWND m_hwnd;
  bool m_isVisible;
  BaseGame *m_pGame;
};

// --------- Inlines ----------
inline void GameWindow::SetSize(const Size &size)
{
  m_size = size;
  SetWindowPos(m_hwnd, 0, 0, 0, size.width, size.height, SWP_NOMOVE);
}

inline const Size &GameWindow::GetSize() const
{
  return m_size;
}

inline void GameWindow::SetTitle(const char *title)
{
  SetWindowText(m_hwnd, title);
}

inline HWND GameWindow::GetWindowHandle() const
{
  return m_hwnd;
}




} // namespace shinybear


#endif