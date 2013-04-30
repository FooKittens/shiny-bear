#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <Windows.h>
#include <XInput.h>

namespace XboxControllerButton
{
  enum Button
  {
    XCB_DPAD_UP = 0x0001,
    XCB_DPAD_DOWN = 0x0002,
    XCB_DPAD_LEFT = 0x0004,
    XCB_DPAD_RIGHT = 0x0008,
    XCB_START = 0x0010,
    XCB_BACK = 0x0020,
    XCB_LEFT_THUMB = 0x0040,
    XCB_RIGHT_THUMB = 0x0080,
    XCB_LEFT_SHOULDER = 0x0100,
    XCB_RIGHT_SHOULDER = 0x0200,
    XCB_GAMEPAD_A = 0x1000,
    XCB_GAMEPAD_B = 0x2000,
    XCB_GAMEPAD_X = 0x4000,
    XCB_GAMEPAD_Y = 0x8000,
  };
}

namespace PlayerIndex
{
  enum PlayerIndex
  {
    PI_ONE = 0,
    PI_TWO = 1,
    PI_THREE = 2,
    PI_FOUR = 3,
  };
}

namespace shinybear {

class XboxController
{
public:
  XboxController(int playerNumber);
  const XINPUT_STATE GetState();
  const bool IsConnected();
  void Vibrate(int leftVal = 0, int rightVal = 0);
private:
  XINPUT_STATE mControllerState;
  int mControllerNum;
};

} // namespace shinybear

#endif