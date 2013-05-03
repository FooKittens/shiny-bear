#ifndef SHINYBEAR_GAMEPADSTATE_H
#define SHINYBEAR_GAMEPADSTATE_H

#include "util\math\Math.h"

#include <climits>
#include <XInput.h>
#include <string>

namespace shinybear
{

namespace GamePadIndex
{
  enum Enum
  {
    ONE = 0,
    TWO = 1,
    THREE = 2,
    FOUR = 3,
  };
} // namespace GamePadIndex

namespace ControllerButtons
{
  enum Enum
  {
    DPAD_UP = 1,
    DPAD_DOWN = 2,
    DPAD_LEFT = 3,
    DPAD_RIGHT = 4,
    START = 5,
    BACK = 6,
    LEFT_THUMB = 7,
    RIGHT_THUMB = 8,
    LEFT_SHOULDER = 9,
    RIGHT_SHOULDER = 10,
    GAMEPAD_A = 11,
    GAMEPAD_B = 12,
    GAMEPAD_X = 13,
    GAMEPAD_Y = 14,
  };
} // namespace ControllerButtons

struct GamePadState
{
public:
  friend class InputManager;
  
  //ctors
  GamePadState(const GamePadIndex::Enum index);
  GamePadState(const GamePadState &gamePadState);

  //dtors
  ~GamePadState();
  
  bool IsButtonDown(const ControllerButtons::Enum button) const;
  void SetGamePadIndex(const GamePadIndex::Enum index);
  
  const float &leftTrigger, &rightTrigger;
  const Vector2 &leftThumbstick, &rightThumbstick;
  const bool &isConnected;

  //takes float values 0.0f - 1.0f
  void Vibrate(const float leftMotor = 0, const float rightMotor = 0);

  //statics
  static const std::string GetButtonName(const ControllerButtons::Enum button);

  //operators
  void operator=(const GamePadState &controllerState);

private:
  // All 14 Xbox Controller buttons:
  // DPAD (4), start, back, thumbsticks (2), shoulders (2), ABXY (4)
  float m_lTrigger, m_rTrigger;
  Vector2 m_lThumb, m_rThumb;

  XINPUT_STATE m_state;
  bool m_connected, m_isVibrating;
  
  GamePadIndex::Enum m_index;

  void Update();
  void ReEvaluateConnection();
};

//inlines
inline void GamePadState::operator=(const GamePadState &gamePadState)
{
  m_lTrigger = gamePadState.m_lTrigger;
  m_rTrigger = gamePadState.m_rTrigger;
  m_lThumb = gamePadState.m_lThumb;
  m_rThumb = gamePadState.m_rThumb;
  m_state = gamePadState.m_state;
}

} // namespace shinybear

#endif // SHINYBEAR_GAMEPADSTATE_H