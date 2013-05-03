#include "util\input\GamePadState.h"

namespace shinybear
{

//ctors
GamePadState::GamePadState(const GamePadIndex::Enum index)
  : leftTrigger(m_lTrigger), rightTrigger(m_rTrigger),
    leftThumbstick(m_lThumb), rightThumbstick(m_rThumb),
    isConnected(m_connected)
{
  m_index = index;
  Update();
}

GamePadState::GamePadState(const GamePadState &gamePadState)
  : leftTrigger(m_lTrigger), rightTrigger(m_rTrigger),
    leftThumbstick(m_lThumb), rightThumbstick(m_rThumb),
    isConnected(m_connected)
{
  m_lTrigger = gamePadState.m_lTrigger;
  m_rTrigger = gamePadState.m_rTrigger;
  m_lThumb = gamePadState.m_lThumb;
  m_rThumb = gamePadState.m_rThumb;

  m_state = gamePadState.m_state;
}

//dtors
GamePadState::~GamePadState()
{
  //stop controller vibration
  Vibrate();
}
  
bool GamePadState::IsButtonDown(const ControllerButtons::Enum button) const
{
  switch(button)
  {
  case ControllerButtons::DPAD_UP:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ? true : false;
  case ControllerButtons::DPAD_DOWN:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? true : false;
  case ControllerButtons::DPAD_LEFT:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? true : false;
  case ControllerButtons::DPAD_RIGHT:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? true : false;
  case ControllerButtons::START:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_START ? true : false;
  case ControllerButtons::BACK:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ? true : false;
  case ControllerButtons::LEFT_THUMB:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ? true : false;
  case ControllerButtons::RIGHT_THUMB:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ? true : false;
  case ControllerButtons::LEFT_SHOULDER:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? true : false;
  case ControllerButtons::RIGHT_SHOULDER:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? true : false;
  case ControllerButtons::GAMEPAD_A:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_A ? true : false;
  case ControllerButtons::GAMEPAD_B:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_B ? true : false;
  case ControllerButtons::GAMEPAD_X:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_X ? true : false;
  case ControllerButtons::GAMEPAD_Y:
    return m_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y ? true : false;
  default:
    return false;
  }
}

void GamePadState::SetGamePadIndex(const GamePadIndex::Enum index)
{
  m_index = index;
}

//takes float values 0.0f - 1.0f
void GamePadState::Vibrate(const float leftMotor, const float rightMotor)
{
  if(m_connected)
  {
    if(abs(leftMotor) < FLT_EPSILON && abs(rightMotor) < FLT_EPSILON)
    {
      if(m_isVibrating)
      {
        m_isVibrating = false;
      }
      else
      {
        return;
      }
    }
    else
    {
      m_isVibrating = true;
    }

    // Create a Vibraton State
    XINPUT_VIBRATION vibration;

    // Set the Vibration Values
    vibration.wLeftMotorSpeed = (unsigned short)(max(0.0f, min(leftMotor, 1.0f)) * USHRT_MAX);
    vibration.wRightMotorSpeed = (unsigned short)(max(0.0f, min(rightMotor, 1.0f)) * USHRT_MAX);

    // Vibrate the controller
    XInputSetState(m_index, &vibration);
  }
}

//statics
const std::string GamePadState::GetButtonName(const ControllerButtons::Enum button)
{
  switch(button)
  {
  case ControllerButtons::DPAD_UP: return "Dpad Up";
  case ControllerButtons::DPAD_DOWN: return "Dpad Down";
  case ControllerButtons::DPAD_LEFT: return "Dpad Down";
  case ControllerButtons::DPAD_RIGHT: return "Dpad Right";
  case ControllerButtons::START: return "Start";
  case ControllerButtons::BACK: return "Back";
  case ControllerButtons::LEFT_THUMB: return "Left thumbstick";
  case ControllerButtons::RIGHT_THUMB: return "Right thumbstick";
  case ControllerButtons::LEFT_SHOULDER: return "Left shoulder";
  case ControllerButtons::RIGHT_SHOULDER: return "Right shoulder";
  case ControllerButtons::GAMEPAD_A: return "A";
  case ControllerButtons::GAMEPAD_B: return "B";
  case ControllerButtons::GAMEPAD_X: return "X";
  case ControllerButtons::GAMEPAD_Y: return "Y";
  }
}

void GamePadState::Update()
{
  if(m_connected)
  {
  // Sets mState
  m_connected = XInputGetState(m_index, &m_state) == ERROR_SUCCESS;

    // Check to make sure we are not moving during the dead zone (LTHUMB)
    if(m_state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	      m_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
    {    
	    m_state.Gamepad.sThumbLX = 0;
    }
    if(m_state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	      m_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
    {
      m_state.Gamepad.sThumbLY = 0;
    }
    m_lThumb.x = m_state.Gamepad.sThumbLX / (float)SHRT_MAX;
    m_lThumb.y = m_state.Gamepad.sThumbLY / (float)SHRT_MAX;

    // Check to make sure we are not moving during the dead zone (RTHUMB)
    if(m_state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	      m_state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
    {    
	    m_state.Gamepad.sThumbRX = 0;
    }
    if(m_state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	      m_state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
    {
      m_state.Gamepad.sThumbRY = 0;
    }
    m_rThumb.x = m_state.Gamepad.sThumbRX / (float)SHRT_MAX;
    m_rThumb.y = m_state.Gamepad.sThumbRY / (float)SHRT_MAX;

    // Check to make sure we are not moving during the dead zone (LTRIGGER)
    if(m_state.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
    {    
	    m_state.Gamepad.bLeftTrigger = 0;
    }
    m_lTrigger = m_state.Gamepad.bLeftTrigger / (float)UCHAR_MAX;
    // Check to make sure we are not moving during the dead zone (RTRIGGER)
    if(m_state.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
    {    
	    m_state.Gamepad.bRightTrigger = 0;
    }
    m_rTrigger = m_state.Gamepad.bRightTrigger / (float)UCHAR_MAX;
  }
}

void GamePadState::ReevaluateConnection()
{
  m_connected = XInputGetState(m_index, &m_state) == ERROR_SUCCESS;
}

} // namespace shinybear