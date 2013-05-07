#include "util\input\MouseState.h"

namespace shinybear
{

MouseState::MouseState(const MouseState &mouseState)
{
  m_mouseButtonStates = mouseState.m_mouseButtonStates;
  m_mouseWheelChange = mouseState.m_mouseWheelChange;
  m_absoluteX = mouseState.m_absoluteX;
  m_absoluteY = mouseState.m_absoluteY;
  m_changeX = mouseState.m_changeX;
  m_changeY = mouseState.m_changeY;
}

bool MouseState::IsButtonDown(const MouseButtons::Enum button) const
{
  return !!(m_mouseButtonStates[button - 1] & 1);
}

Vector2 MouseState::GetPositionalChange() const
{
  return Vector2(m_changeX, m_changeY);
}

const std::string MouseState::GetButtonName(const MouseButtons::Enum button)
{
  switch(button)
  {
    // TODO string table
  case MouseButtons::MB_LEFTMOUSE: return "Left Mouse";
  case MouseButtons::MB_RIGHTMOUSE: return "Right Mouse";
  case MouseButtons::MB_MIDDLEMOUSE: return "Middle Mouse";
  case MouseButtons::MB_XBUTTON1: return "X1 Mouse";
  case MouseButtons::MB_XBUTTON2: return "X2 Mouse";
  default: return "OEM Special";
  }
}

} // namespace shinybear