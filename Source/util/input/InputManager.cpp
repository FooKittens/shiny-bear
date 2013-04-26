#include "util\input\InputManager.h"
#include "base\system\GameWindow.h"

#include <Windows.h>
#include <cassert>

namespace shinybear 
{

int InputManager::s_keys[InputManager::kKeyCount] = { 0 };


void InputManager::Initialize(const GameWindow &pGameWindow)
{
  HWND handle = pGameWindow.GetWindowHandle();

  RAWINPUTDEVICE Rid[3];
  
  Rid[0].usUsagePage = 0x01;
  Rid[0].usUsage = 0x05;
  Rid[0].dwFlags = 0; // adds game pad
  Rid[0].hwndTarget = handle;

  Rid[1].usUsagePage = 0x01;
  Rid[1].usUsage = 0x02;
  Rid[1].dwFlags = RIDEV_NOLEGACY; // adds HID mouse and also ignores legacy mouse messages
  Rid[1].hwndTarget = handle;

  Rid[2].usUsagePage = 0x01;
  Rid[2].usUsage = 0x06;
  Rid[2].dwFlags = RIDEV_NOLEGACY; // adds HID keyboard and also ignores legacy keyboard messages
  Rid[2].hwndTarget = handle;

  assert (RegisterRawInputDevices(Rid, 3, sizeof(Rid[0])) &&
    "Failed to register for raw input!");

  return;
}

void InputManager::GetKeyboardState(KeyboardState *pState) 
{
  // Safe, we wont' be changing the copy at all.
  int *pk = s_keys, *pc = pState->keys;
  
  for(int i = 0; i < kKeyCount; ++i)
  {
    *pc++ = *pk++;
  }
}

void InputManager::HandleInput(const HRAWINPUT &hInput)
{
  UINT size = 0;
  if (GetRawInputData(hInput, RID_HEADER, NULL, &size, sizeof(RAWINPUTHEADER)))
  {
    assert(false && "Failed to get raw input header!");
  }

  LPVOID data = nullptr;

  if (GetRawInputData(hInput, RID_INPUT, data, &size, sizeof(RAWINPUTHEADER)))
  {
    assert(false && "Failed to get raw input data!");
  }

  assert("WOHOO");
}

//LRESULT CALLBACK InputManager::KeyboardHook(int code, WPARAM w, LPARAM l)
//{
//  if(code < 0)
//  {
//    return CallNextHookEx(s_hookHandle, code, w, l);
//  }
//
//  if(code == HC_ACTION)
//  {
//    // Transition state is saved in the 32nd bit. 1 Means key is being pressed.
//    s_keys[w] = !(l >> 31);
//  }
//
//  return CallNextHookEx(s_hookHandle, code, w, l);
//}

} // namespace framework