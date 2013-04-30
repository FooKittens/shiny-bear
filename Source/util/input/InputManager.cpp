#include "util\input\InputManager.h"
#include "base\system\GameWindow.h"

#include <Windows.h>
#include <cassert>
#include <bitset>

namespace shinybear 
{
  // initialization of static variables
  MouseState InputManager::mouseState = MouseState();
  KeyboardState InputManager::keyboardState = KeyboardState();
  std::bitset<255> InputManager::tempKey(1);

void InputManager::Initialize(const GameWindow &pGameWindow)
{
  // This method registers the application as listener to raw input
  HWND handle = pGameWindow.GetWindowHandle();
  RAWINPUTDEVICE Rid[3];
  
  // http://www.usb.org/developers/devclass_docs/Hut1_11.pdf page 27

  // adds game pad
  Rid[0].usUsagePage = 0x01; // Generic Desktop Device
  Rid[0].usUsage = 0x05; // Game Pad
  Rid[0].dwFlags = 0;
  Rid[0].hwndTarget = handle;

  // adds HID mouse
  Rid[1].usUsagePage = 0x01; // Generic Desktop Device
  Rid[1].usUsage = 0x02; // Mouse
  Rid[1].dwFlags = RIDEV_NOLEGACY;
  Rid[1].hwndTarget = handle;

  // adds HID keyboard
  Rid[2].usUsagePage = 0x01; // Generic Desktop Device
  Rid[2].usUsage = 0x06; // Keyboard
  Rid[2].dwFlags = RIDEV_NOLEGACY;
  Rid[2].hwndTarget = handle;

  if(!RegisterRawInputDevices(Rid, 3, sizeof(Rid[0])))
  {
    assert (false && "Failed to register for raw input!");
  }

  return;
}

void InputManager::HandleInput(const HRAWINPUT &hInput)
{
  // Find size of rawinput
  UINT dwSize;
  GetRawInputData(hInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
  LPBYTE lpb = new BYTE[dwSize];
  if(lpb == NULL)
  {
    // If size is zero, exit (should never happen though)
    return;
  }

  //
  if(GetRawInputData(hInput, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
  {
    assert(false && "GetRawInputData does not return correct size!");
  }

  RAWINPUT *pRawInput = (RAWINPUT*)lpb;

  switch(pRawInput->header.dwType)
  {
    case RIM_TYPEKEYBOARD:
    {
      // Get raw data
      unsigned short scanCode = pRawInput->data.keyboard.MakeCode;
      unsigned short flags = pRawInput->data.keyboard.Flags;
      unsigned short vKey= pRawInput->data.keyboard.VKey;
      
      // Begin fix of bogus virtual keys
      if(vKey == 255)
      {
        // Discard "fake keys" which are part of an escaped sequence
        return;
      }
      else if(vKey == VK_SHIFT)
      {
        // Correct left-hand / right-hand SHIFT
        vKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
      }
      else if(vKey == VK_NUMLOCK)
      {
        // Correct PAUSE/BREAK and NUM LOCK indistinguishability, and set the extended bit
        scanCode = (MapVirtualKey(vKey, MAPVK_VK_TO_VSC) | 0x100);
      }

      // Find escape sequence flags, to differ between ambiguous keys,
      // for instance Numpad7 vs Home or LCtrl vs RCtrl
      const bool isE0 = ((flags & RI_KEY_E0) != 0);
      const bool isE1 = ((flags & RI_KEY_E1) != 0);
 
      if(isE1)
      {
        // For escaped sequences, turn the virtual key into the correct scan code with MapVirtualKey.
        // MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
        if (vKey == VK_PAUSE)
          scanCode = 0x45;
        else
          scanCode = MapVirtualKey(vKey, MAPVK_VK_TO_VSC);
      }

      // Make sure ambiguous VirtualKeys are always specific
      // for instance Numpad7 vs Home or LCtrl vs RCtrl
      switch (vKey)
      {
        // right-hand CONTROL and ALT have their e0 bit set
        case VK_CONTROL:
          if (isE0)
            vKey = Keyboard::K_RIGHTCTRL;
          else
            vKey = Keyboard::K_LEFTCTRL;
          break;

        case VK_MENU:
          if (isE0)
            vKey = Keyboard::K_RIGHTALT;
          else
            vKey = Keyboard::K_LEFTALT;
          break;
 
        // NUMPAD ENTER has its e0 bit set
        case VK_RETURN:
          if (isE0)
            vKey = Keyboard::K_SEPARATOR;
          break;
 
        // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always
        // have their e0 bit set, but the corresponding keys on the NUMPAD will not.
        case VK_INSERT:
          if (!isE0)
          vKey = Keyboard::K_NUMPAD0;
          break;
 
        case VK_DELETE:
          if (!isE0)
            vKey = Keyboard::K_DECIMAL;
          break;
 
        case VK_HOME:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD7;
          break;
 
        case VK_END:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD1;
          break;
 
        case VK_PRIOR:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD9;
          break;
 
        case VK_NEXT:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD3;
          break;
 
        // the standard arrow keys will always have their e0 bit set,
        // but the corresponding keys on the NUMPAD will not.
        case VK_LEFT:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD4;
          break;

        case VK_RIGHT:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD6;
          break;
 
        case VK_UP:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD8;
          break;
 
        case VK_DOWN:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD2;
          break;
 
        // NUMPAD 5 doesn't have its e0 bit set
        case VK_CLEAR:
          if (!isE0)
            vKey = Keyboard::K_NUMPAD5;
          break;
      }

      if(flags & RI_KEY_BREAK) // Key was released
      {
        if(vKey < 255) // 255 should already have been handled as a fake key
        {
          // tempKey is a bitset<255>, just like keyStates
          tempKey = 1;
          tempKey <<= (vKey - 1);
          keyboardState.keyStates &= ~tempKey; // Sets the vKey'th bit to 0
        }
        else
        {
          assert(false && "Invalid virtual key received!");
        }
      }
      else // Key was pressed
      {
        if(vKey < 255) // 255 should already have been handled as a fake key
        {
          // tempKey is a bitset<255>, just like keyStates
          tempKey = 1;
          tempKey <<= (vKey - 1);
          keyboardState.keyStates |= tempKey; // Sets the vKey'th bit to 1
        }
        else
        {
          assert(false && "Invalid virtual key received!");
        }
      }

      break;
    } // END OF KEYBOARD INPUT HANDLING
    case RIM_TYPEMOUSE:
    {

      unsigned short flags = pRawInput->data.mouse.usFlags;
      unsigned short buttonFlags = pRawInput->data.mouse.usButtonFlags;
      long lastX = pRawInput->data.mouse.lLastX;
      long lastY = pRawInput->data.mouse.lLastY;

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
        mouseState.mouseWheelChange = static_cast<short>(pRawInput->data.mouse.usButtonData);
      }
      else
      {
        if (buttonFlags & RI_MOUSE_BUTTON_1_DOWN)
          mouseState.mouseButtonStates |= (1 << 0); // Sets the 1nd bit to 1
        if (buttonFlags & RI_MOUSE_BUTTON_1_UP)
          mouseState.mouseButtonStates &= ~(1 << 0); // Sets the 1nd bit to 0

        if (buttonFlags & RI_MOUSE_BUTTON_2_DOWN)
          mouseState.mouseButtonStates |= (1 << 1); // Sets the 2nd bit to 1
        if (buttonFlags & RI_MOUSE_BUTTON_2_UP)
          mouseState.mouseButtonStates &= ~(1 << 1); // Sets the 2nd bit to 0

        // Third bit is ignored due to the fact that Microsoft's
        // 3rd VK_-define, VK_CANCEL, is not a mouse-related virtual key

        if (buttonFlags & RI_MOUSE_BUTTON_3_DOWN)
          mouseState.mouseButtonStates |= (1 << 3); // Sets the 4th bit to 1
        if (buttonFlags & RI_MOUSE_BUTTON_3_UP)
          mouseState.mouseButtonStates &= ~(1 << 3); // Sets the 4th bit to 0

        if (buttonFlags & RI_MOUSE_BUTTON_4_DOWN)
          mouseState.mouseButtonStates |= (1 << 4); // Sets the 5th bit to 1
        if (buttonFlags & RI_MOUSE_BUTTON_4_UP)
          mouseState.mouseButtonStates &= ~(1 << 4); // Sets the 5th bit to 0

        if (buttonFlags & RI_MOUSE_BUTTON_5_DOWN)
          mouseState.mouseButtonStates |= (1 << 5); // Sets the 6th bit to 1
        if (buttonFlags & RI_MOUSE_BUTTON_5_UP)
          mouseState.mouseButtonStates &= ~(1 << 5); // Sets the 6th bit to 0
      }

      break;
    } // END OF MOUSE INPUT HANDLING
    case RIM_TYPEHID:
    {
      BYTE *data = pRawInput->data.hid.bRawData;
      unsigned long sizePerReport = pRawInput->data.hid.dwSizeHid;
      unsigned long reportCount = pRawInput->data.hid.dwCount;

      unsigned int bufferSize = sizePerReport * reportCount;

      break;
    } // END OF GAMEPAD INPUT HANDLING
  }

  delete lpb;

  return;
}

} // namespace framework