#include "view\input\InputManager.h"
#include <Windows.h>
#include <cassert>

namespace shinybear 
{

HHOOK InputManager::s_hookHandle = 0;
int InputManager::s_keys[InputManager::kKeyCount] = { 0 };


void InputManager::Initialize()
{
  // Register wndproc hook
  s_hookHandle = SetWindowsHookEx(
    WH_CALLWNDPROC,
    &InputManager::InputHook,
    GetModuleHandle(NULL),
    GetCurrentThreadId()
  );

  assert(s_hookHandle && "Failed to attach wndproc hook!");
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

LRESULT CALLBACK InputManager::InputHook(int code, WPARAM w, LPARAM l)
{
  if(code < 0)
  {
    return CallNextHookEx(s_hookHandle, code, w, l);
  }

  if(code == HC_ACTION)
  {
    LPCWPSTRUCT pWndproc = (CWPSTRUCT*)l;

    switch (pWndproc->message)
    {
      // Register for raw input
      case WM_CREATE:
      {
        RAWINPUTDEVICE Rid[2];
        // Keyboard
        Rid[0].usUsagePage = 1;
        Rid[0].usUsage = 6;
        Rid[0].dwFlags = 0;
        Rid[0].hwndTarget = pWndproc->hwnd;

        // Mouse
        Rid[1].usUsagePage = 1;
        Rid[1].usUsage = 2;
        Rid[1].dwFlags = 0;
        Rid[1].hwndTarget = pWndproc->hwnd;
        if (!RegisterRawInputDevices(Rid,2,sizeof(RAWINPUTDEVICE)))
        {
          // TODO error handling
        }
        return CallNextHookEx(s_hookHandle, code, w, l);
      }

      case WM_INPUT:
      {
        if (pWndproc->wParam == RIM_INPUT) // window was in foreground
        {
          LPRAWINPUT input = (RAWINPUT*)pWndproc->lParam;
          return CallNextHookEx(s_hookHandle, code, w, l);
        }
      }
    } // wndproc
  }

  return CallNextHookEx(s_hookHandle, code, w, l);
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