#include "XboxController.h"

namespace shinybear
{

XboxController::XboxController(int playerNumber)
{
  // Set the Controller Number
  mControllerNum = playerNumber - 1;
}

const XINPUT_STATE XboxController::GetState()
{
  // Zeroise the state
  ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));

  // Get the state
  XInputGetState(mControllerNum, &mControllerState);
  
  return mControllerState;
}

const bool XboxController::IsConnected()
{
  // Zeroise the state
  ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));

  // Get the state
  DWORD Result = XInputGetState(mControllerNum, &mControllerState);

  if(Result == ERROR_SUCCESS)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void XboxController::Vibrate(int leftVal, int rightVal)
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(mControllerNum, &Vibration);
}

} // namespace shinybear