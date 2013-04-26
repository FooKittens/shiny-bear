#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <windows.h>

namespace shinybear { class BaseGame; class GameWindow; }

namespace shinybear
{

enum Keys
{
  A = 'A',
  B = 'B',
  C = 'C',
  D = 'D',
  E = 'E',
  F = 'F',
  G = 'G',
  H = 'H',
  I = 'I',
  J = 'J',
  K = 'K',
  L = 'L',
  M = 'M',
  N = 'N',
  O = 'O',
  P = 'P',
  Q = 'Q',
  R = 'R',
  S = 'S',
  T = 'T',
  U = 'U',
  V = 'V',
  W = 'W',
  X = 'X',
  Z = 'Z',
  Space = VK_SPACE,
  Alt = VK_MENU,
  Ctrl = VK_CONTROL,
  Tab = VK_TAB,
  CapsLock = VK_CAPITAL,
  Shift = VK_SHIFT,
  D0 = 30,
  D1 = 31,
  D2 = 32,
  D3 = 33,
  D4 = 34,
  D5 = 35,
  D6 = 36,
  D7 = 37,
  D8 = 38,
  D9 = 39,
  Numpad0 = VK_NUMPAD0,
  Numpad1 = VK_NUMPAD1,
  Numpad2 = VK_NUMPAD2,
  Numpad3 = VK_NUMPAD3,
  Numpad4 = VK_NUMPAD4,
  Numpad5 = VK_NUMPAD5,
  Numpad6 = VK_NUMPAD6,
  Numpad7 = VK_NUMPAD7,
  Numpad8 = VK_NUMPAD8,
  Numpad9 = VK_NUMPAD9,
  Add = VK_ADD,
  Subtract = VK_SUBTRACT,
  Divide = VK_DIVIDE,
  Multiply = VK_MULTIPLY,
  Del = VK_DELETE,
  Insert = VK_INSERT,
  Home = VK_HOME,
  PageDown = VK_NEXT,
  PageUp = VK_PRIOR,
  Esc = VK_ESCAPE,
  End = VK_END,
  F1 = VK_F1,
  F2 = VK_F2,
  F3 = VK_F3,
  F4 = VK_F4,
  F5 = VK_F5,
  F6 = VK_F6,
  F7 = VK_F7,
  F8 = VK_F8,
  F9 = VK_F9,
  F10 = VK_F10,
  F11 = VK_F11,
  F12 = VK_F12,
  Return = VK_RETURN,
  Left = VK_LEFT,
  Right = VK_RIGHT,
  Up = VK_UP,
  Down = VK_DOWN,
};

struct MouseState
{
public:
  bool leftBtnDown;
  bool middleBtnDown;
  bool rightBtnDown;
  bool xBtn1Down;
  bool xBtn2Down;
  short mouseWheelChange;
  // Absolute mouse position, according to windows
  int absoluteX;
  int absoluteY;
  // Mouse XY change since previous event
  int changeX;
  int changeY;
};

class InputManager
{

public:
  friend class BaseGame;
  friend class GameWindow;

private:
  static void Initialize(const GameWindow &pGameWindow);

  static void HandleInput(const HRAWINPUT &hInput);

  static MouseState mouseState;
};

} // namespace framework

#endif