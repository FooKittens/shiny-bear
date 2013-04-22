#include "view\input\InputManager.h"
#include <Windows.h>
#include <cassert>

namespace framework 
{

HHOOK InputManager::s_hookHandle = 0;
int InputManager::s_keys[InputManager::kKeyCount] = { 0 };


void InputManager::Initialize()
{
  
  s_hookHandle = SetWindowsHookEx(
    WH_KEYBOARD,
    &InputManager::KeyboardHook,
    GetModuleHandle(NULL),
    GetCurrentThreadId()
  );

  assert(s_hookHandle && "Failed to attach keyboard hook!");
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

LRESULT CALLBACK InputManager::KeyboardHook(int code, WPARAM w, LPARAM l)
{
  if(code < 0)
  {
    return CallNextHookEx(s_hookHandle, code, w, l);
  }

  if(code == HC_ACTION)
  {
    // Transition state is saved in the 32nd bit. 1 Means key is being pressed.
    s_keys[w] = !(l >> 31);
  }

  return CallNextHookEx(s_hookHandle, code, w, l);
}






} // namespace framework