#ifndef SHINYBEAR_MOUSESTATE_H
#define SHINYBEAR_MOUSESTATE_H

#include "util\math\Math.h"

#include <bitset>
#include <Windows.h>
#include <string>

namespace shinybear
{

namespace MouseButtons
{
enum Enum
{
  MB_LEFTMOUSE = VK_LBUTTON, // 0x01
  MB_RIGHTMOUSE = VK_RBUTTON, // 0x02
  // 0x03 not a VK_-mouse button
  MB_MIDDLEMOUSE = VK_MBUTTON, // 0x04
  MB_XBUTTON1 = VK_XBUTTON1, // 0x05
  MB_XBUTTON2 = VK_XBUTTON2, // 0x06
};
} // namespace MouseButtons

struct MouseState
{
public:
  friend class InputManager;

  //ctors
  MouseState(){}
  MouseState(const MouseState &mouseState);

  //mouse checks
  bool IsButtonDown(const MouseButtons::Enum button) const;
  Vector2 GetPositionalChange() const;

  //statics
  static const std::string GetButtonName(const MouseButtons::Enum button);

  //operators
  void operator=(const MouseState &mouseState);

private:
  // size 6 to match Microsoft's VK_-defines, where 0x03 is a
  // non-mouse related VK, with 1,2,4,5,6 being mouse buttons
  std::bitset<6> m_mouseButtonStates;
  // Mouse wheel change
  short m_mouseWheelChange;
  // Absolute mouse position, according to windows
  int m_absoluteX, m_absoluteY;
  // Mouse XY change since previous event
  int m_changeX, m_changeY;
};

inline void MouseState::operator=(const MouseState &mouseState)
{
  m_mouseButtonStates = mouseState.m_mouseButtonStates;
  m_mouseWheelChange = mouseState.m_mouseWheelChange;
  m_absoluteX = mouseState.m_absoluteX;
  m_absoluteY = mouseState.m_absoluteY;
  m_changeX = mouseState.m_changeX;
  m_changeY = mouseState.m_changeY;
}

} // namespace shinybear

#endif // SHINYBEAR_MOUSESTATE_H