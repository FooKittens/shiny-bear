#include "util\input\InputManager.h"
#include "base\system\GameWindow.h"

#include <Windows.h>
#include <cassert>

namespace shinybear 
{

  MouseState InputManager::mouseState = MouseState();
//int InputManager::s_keys[InputManager::kKeyCount] = { 0 };

void InputManager::Initialize(const GameWindow &pGameWindow)
{
  HWND handle = pGameWindow.GetWindowHandle();

  RAWINPUTDEVICE Rid[3];
  
  // adds game pad
  Rid[0].usUsagePage = 0x01;
  Rid[0].usUsage = 0x05;
  Rid[0].dwFlags = 0;
  Rid[0].hwndTarget = handle;

  // adds HID mouse
  Rid[1].usUsagePage = 0x01;
  Rid[1].usUsage = 0x02;
  Rid[1].dwFlags = 0;
  Rid[1].hwndTarget = handle;

  // adds HID keyboard
  Rid[2].usUsagePage = 0x01;
  Rid[2].usUsage = 0x06;
  Rid[2].dwFlags = 0;
  Rid[2].hwndTarget = handle;

  assert (RegisterRawInputDevices(Rid, 3, sizeof(Rid[0])) &&
    "Failed to register for raw input!");

  return;
}

void InputManager::HandleInput(const HRAWINPUT &hInput)
{
  UINT dwSize;

  GetRawInputData(hInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
  LPBYTE lpb = new BYTE[dwSize];
  if(lpb == NULL)
  {
    return;
  }

  if(GetRawInputData(hInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
  {
    assert(false && "GetRawInputData does not return correct size!");
  }

  RAWINPUT *raw = (RAWINPUT*)lpb;

  switch (raw->header.dwType)
  {
    case RIM_TYPEKEYBOARD:
    {
      unsigned short makeCode = raw->data.keyboard.MakeCode;
      unsigned short flags = raw->data.keyboard.Flags;
      unsigned short vKey= raw->data.keyboard.VKey;
      
      if(flags & 1)
      {
        assert(false);
      }
      break;
    }
    case RIM_TYPEMOUSE:
    {
      unsigned short flags = raw->data.mouse.usFlags;
      unsigned short buttonFlags = raw->data.mouse.usButtonFlags;
      long lastX = raw->data.mouse.lLastX;
      long lastY = raw->data.mouse.lLastY;

      // Check mouse movement parameters
      if(flags == MOUSE_MOVE_RELATIVE)
      {
        mouseState.changeX = static_cast<int>(lastX);
        mouseState.changeY = static_cast<int>(lastY);
      }
      else if (flags & MOUSE_MOVE_ABSOLUTE)
      {
        mouseState.absoluteX = static_cast<int>(lastX);
        mouseState.absoluteY = static_cast<int>(lastY);
        if (flags & MOUSE_VIRTUAL_DESKTOP)
        {
          assert(false && "Your mouse entered a virtual screen!");
        }
      }

      // Check button or scroll wheel status
      if(buttonFlags & RI_MOUSE_WHEEL)
      {
        mouseState.mouseWheelChange = static_cast<short>(raw->data.mouse.usButtonData);
      }
      else
      {
        if (buttonFlags & RI_MOUSE_BUTTON_1_DOWN)
          mouseState.leftBtnDown = true;
        if (buttonFlags & RI_MOUSE_BUTTON_1_UP)
          mouseState.leftBtnDown = false;

        if (buttonFlags & RI_MOUSE_BUTTON_2_DOWN)
          mouseState.middleBtnDown = true;
        if (buttonFlags & RI_MOUSE_BUTTON_2_UP)
          mouseState.middleBtnDown = false;

        if (buttonFlags & RI_MOUSE_BUTTON_3_DOWN)
          mouseState.rightBtnDown = true;
        if (buttonFlags & RI_MOUSE_BUTTON_3_UP)
          mouseState.rightBtnDown = false;

        if (buttonFlags & RI_MOUSE_BUTTON_4_DOWN)
          mouseState.xBtn1Down = true;
        if (buttonFlags & RI_MOUSE_BUTTON_4_UP)
          mouseState.xBtn1Down = false;

        if (buttonFlags & RI_MOUSE_BUTTON_5_DOWN)
          mouseState.xBtn2Down = true;
        if (buttonFlags & RI_MOUSE_BUTTON_5_UP)
          mouseState.xBtn2Down = false;
      }

      break;
    }
    case RIM_TYPEHID:
    {
      BYTE *data = raw->data.hid.bRawData;
      unsigned long sizePerReport = raw->data.hid.dwSizeHid;
      unsigned long reportCount = raw->data.hid.dwCount;

      break;
    }
  }

  delete lpb;

  return;
}

} // namespace framework