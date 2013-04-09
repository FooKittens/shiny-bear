#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <windows.h>

namespace framework { struct KeyboardState; }

namespace framework {

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

class InputManager {

public:
  static const int kKeyCount = 256;

  static void Initialize();

  inline static bool IsKeyDown(Keys k) {
    return static_cast<bool>(s_keys[k]);
  }

  inline static bool IsKeyUp(Keys k) {
    return static_cast<bool>(!s_keys[k]);
  }

  // Pass in a pointer to an integer in sizeNeeded
  // to find out the required size of pKeys.
  // If the size is not >= kKeyCount the function will do nothing.
  static void GetKeyboardState(KeyboardState *pState);
private:
  

  static LRESULT CALLBACK KeyboardHook(int keycode, WPARAM wparam, LPARAM lparam);
  static int s_keys[kKeyCount];
  static HHOOK s_hookHandle;
};

struct KeyboardState {

public:
  friend class InputManager;

  KeyboardState()
    :keys(){

    memset(keys, 0, sizeof(int) * InputManager::kKeyCount);
  }

  KeyboardState(const KeyboardState &copy) {
    // Safe, we wont' be changing the copy at all.
    int *pk = keys, *pc = const_cast<KeyboardState&>(copy).keys;
  
    for(int i = 0; i < InputManager::kKeyCount; ++i) {
      *pk++ = *pc++;
    }
  }

  const KeyboardState& operator=(const KeyboardState &rhs) {
    // Safe, we wont' be changing the copy at all.
    int *pk = keys, *pc = const_cast<KeyboardState&>(rhs).keys;
  
    for(int i = 0; i < InputManager::kKeyCount; ++i) {
      *pk++ = *pc++;
    }

    return *this;
  }

  inline bool IsKeyDown(Keys k) { return static_cast<bool>(keys[k]); }
  inline bool IsKeyUp(Keys k) { return static_cast<bool>(!keys[k]); }

private:
  int keys[InputManager::kKeyCount];
};


} // namespace framework

#endif