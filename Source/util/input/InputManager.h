#ifndef SHINYBEAR_INPUTMANAGER_H
#define SHINYBEAR_INPUTMANAGER_H

#include "util\input\MouseState.h"
#include "util\input\KeyboardState.h"
#include "util\input\GamePadState.h"

#include <windows.h>
#include <bitset>
#include <string>
#include <cassert>


namespace shinybear { class BaseGame; class GameWindow; }

namespace shinybear
{

class InputManager
{
public:
  friend class BaseGame;
  friend class GameWindow;

  static void GetKeyboardState(KeyboardState * const);
  static void GetMouseState(MouseState * const);
  static void GetControllerState(GamePadState * const);

private:
  static void Initialize(const GameWindow &pGameWindow);

  static void Update();
  static void ReEvaluateGamePadConnections();
  static void HandleInput(const HRAWINPUT &);

  static MouseState mouseState;
  static bool overrideMouseChange;
  static KeyboardState keyboardState;
  static const int kMaximumGamePadCount = 4;
  static GamePadState gamePads[kMaximumGamePadCount];
  static std::bitset<255> tempKey;
  static bool isInitialized;
};

// Inlines
inline void InputManager::GetKeyboardState(KeyboardState * const pState)
{
  assert(pState);
  *pState = keyboardState;
}

inline void InputManager::GetMouseState(MouseState * const pState)
{
  assert(pState);
  *pState = mouseState;
}

inline void InputManager::GetControllerState(GamePadState * const pState)
{
  assert(pState);
  *pState = gamePads[pState->m_index];
}

} // namespace shinybear

#endif // SHINYBEAR_INPUTMANAGER_H