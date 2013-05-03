#include "util\input\KeyboardState.h"

namespace shinybear
{

KeyboardState::KeyboardState(){}
KeyboardState::KeyboardState(const KeyboardState &keyState)
{
  m_keyStates = keyState.m_keyStates;
}

bool KeyboardState::IsKeyDown(const Keys::Enum key) const
{
  return !!(m_keyStates[key - 1] & 1);
}

bool KeyboardState::IsAnyKeyDown() const
{
  for(int i = 0; i != 255; ++i)
  {
    if(!!(m_keyStates[i] & 1))
    {
      return true;
    }
  }
  return false;
}

const std::string KeyboardState::GetKeyName(const Keys::Enum key)
{
  // TODO string table
  switch(key)
  {
  case Keys::K_CONTROLBREAK: return "Control Break";
  case Keys::K_BACKSPACE: return "Backspace";
  case Keys::K_TAB: return "Tab";
  case Keys::K_CLEAR: return "Clear";
  case Keys::K_ENTER: return "Enter";
  case Keys::K_SHIFT: return "Shift";
  case Keys::K_CTRL: return "Ctrl";
  case Keys::K_ALT: return "Alt";
  case Keys::K_PAUSE: return "Pause";
  case Keys::K_CAPSLOCK: return "Caps Lock";
  case Keys::K_KANA: return "Kana";
  case Keys::K_JUNJA: return "Junja";
  case Keys::K_FINAL: return "Final";
  case Keys::K_KANJI: return "Hanja / Kanji";
  case Keys::K_ESCAPE: return "Escape";
  case Keys::K_CONVERT: return "Convert";
  case Keys::K_NONCONVERT: return "Non-convert";
  case Keys::K_ACCEPT: return "Accept";
  case Keys::K_MODECHANGE: return "Mode change";
  case Keys::K_SPACEBAR: return "Space";
  case Keys::K_PAGEUP: return "Page Up";
  case Keys::K_PAGEDOWN: return "Page Down";
  case Keys::K_END: return "End";
  case Keys::K_HOME: return "Home";
  case Keys::K_LEFT: return "Left";
  case Keys::K_UP: return "Up";
  case Keys::K_RIGHT: return "Right";
  case Keys::K_DOWN: return "Down";
  case Keys::K_SELECT: return "Select";
  case Keys::K_PRINT: return "Print";
  case Keys::K_EXECUTE: return "Execute";
  case Keys::K_PRINTSCREEN: return "Print Screen";
  case Keys::K_INSERT: return "Insert";
  case Keys::K_DELETE: return "Delete";
  case Keys::K_HELP: return "Help";
  case Keys::K_0: return "0";
  case Keys::K_1: return "1";
  case Keys::K_2: return "2";
  case Keys::K_3: return "3";
  case Keys::K_4: return "4";
  case Keys::K_5: return "5";
  case Keys::K_6: return "6";
  case Keys::K_7: return "7";
  case Keys::K_8: return "8";
  case Keys::K_9: return "9";
  case Keys::K_A: return "A";
  case Keys::K_B: return "B";
  case Keys::K_C: return "C";
  case Keys::K_D: return "D";
  case Keys::K_E: return "E";
  case Keys::K_F: return "F";
  case Keys::K_G: return "G";
  case Keys::K_H: return "H";
  case Keys::K_I: return "I";
  case Keys::K_J: return "J";
  case Keys::K_K: return "K";
  case Keys::K_L: return "L";
  case Keys::K_M: return "M";
  case Keys::K_N: return "N";
  case Keys::K_O: return "O";
  case Keys::K_P: return "P";
  case Keys::K_Q: return "Q";
  case Keys::K_R: return "R";
  case Keys::K_S: return "S";
  case Keys::K_T: return "T";
  case Keys::K_U: return "U";
  case Keys::K_V: return "V";
  case Keys::K_W: return "W";
  case Keys::K_X: return "X";
  case Keys::K_Y: return "Y";
  case Keys::K_Z: return "Z";
  case Keys::K_LEFTWINDOWS: return "Left Windows";
  case Keys::K_RIGHTWINDOWS: return "Right Windows";
  case Keys::K_APPLICATIONS: return "Applications";
  case Keys::K_SLEEP: return "Sleep";
  case Keys::K_NUMPAD0: return "Numpad 0";
  case Keys::K_NUMPAD1: return "Numpad 1";
  case Keys::K_NUMPAD2: return "Numpad 2";
  case Keys::K_NUMPAD3: return "Numpad 3";
  case Keys::K_NUMPAD4: return "Numpad 4";
  case Keys::K_NUMPAD5: return "Numpad 5";
  case Keys::K_NUMPAD6: return "Numpad 6";
  case Keys::K_NUMPAD7: return "Numpad 7";
  case Keys::K_NUMPAD8: return "Numpad 8";
  case Keys::K_NUMPAD9: return "Numpad 9";
  case Keys::K_MULTIPLY: return "Numpad *";
  case Keys::K_ADD: return "Numpad +";
  case Keys::K_SEPARATOR: return "Numpad Enter";
  case Keys::K_SUBTRACT: return "Numpad -";
  case Keys::K_DECIMAL: return "Numpad .";
  case Keys::K_DIVIDE: return "Numpad /";
  case Keys::K_F1: return "F1";
  case Keys::K_F2: return "F2";
  case Keys::K_F3: return "F3";
  case Keys::K_F4: return "F4";
  case Keys::K_F5: return "F5";
  case Keys::K_F6: return "F6";
  case Keys::K_F7: return "F7";
  case Keys::K_F8: return "F8";
  case Keys::K_F9: return "F9";
  case Keys::K_F10: return "F10";
  case Keys::K_F11: return "F11";
  case Keys::K_F12: return "F12";
  case Keys::K_F13: return "F13";
  case Keys::K_F14: return "F14";
  case Keys::K_F15: return "F15";
  case Keys::K_F16: return "F16";
  case Keys::K_F17: return "F17";
  case Keys::K_F18: return "F18";
  case Keys::K_F19: return "F19";
  case Keys::K_F20: return "F20";
  case Keys::K_F21: return "F21";
  case Keys::K_F22: return "F22";
  case Keys::K_F23: return "F23";
  case Keys::K_F24: return "F24";
  case Keys::K_NUMLOCK: return "Num Lock";
  case Keys::K_SCROLLLOCK: return "Scroll Lock";
  case Keys::K_JISHO: return "Jisho";
  case Keys::K_MASSHOU: return "Mashu";
  case Keys::K_TOUROKU: return "Touroku";
  case Keys::K_LOYA: return "Loya";
  case Keys::K_ROYA: return "Roya";
  case Keys::K_LEFTSHIFT: return "Left Shift";
  case Keys::K_RIGHTSHIFT: return "Right Shift";
  case Keys::K_LEFTCTRL: return "Left Ctrl";
  case Keys::K_RIGHTCTRL: return "Right Ctrl";
  case Keys::K_LEFTALT: return "Left Alt";
  case Keys::K_RIGHTALT: return "Right Alt";
  case Keys::K_BROWSER_BACK: return "Browser Back";
  case Keys::K_BROWSER_FORWARD: return "Browser Forward";
  case Keys::K_BROWSER_REFRESH: return "Browser Refresh";
  case Keys::K_BROWSER_STOP: return "Browser Stop";
  case Keys::K_BROWSER_SEARCH: return "Browser Search";
  case Keys::K_BROWSER_FAVORITES: return "Browser Favorites";
  case Keys::K_BROWSER_HOME: return "Browser Home";
  case Keys::K_VOLUME_MUTE: return "Volume Mute";
  case Keys::K_VOLUME_DOWN: return "Volume Down";
  case Keys::K_VOLUME_UP: return "Volume Up";
  case Keys::K_MEDIA_NEXT_TRACK: return "Next Track";
  case Keys::K_MEDIA_PREV_TRACK: return "Previous Track";
  case Keys::K_MEDIA_STOP: return "Stop Media";
  case Keys::K_MEDIA_PLAY_PAUSE: return "Play/Pause";
  case Keys::K_LAUNCH_MAIL: return "Launch Mail";
  case Keys::K_LAUNCH_MEDIA_SELECT: return "Select Media";
  case Keys::K_LAUNCH_APP1: return "Launch Application 1";
  case Keys::K_LAUNCH_APP2: return "Launch Application 2";
  case Keys::K_OEM_1: return "OEM 1 (; :)";
  case Keys::K_OEM_PLUS: return "OEM Plus (= +)";
  case Keys::K_OEM_COMMA: return "OEM Comma (, <)";
  case Keys::K_OEM_MINUS: return "OEM Minus (- _)";
  case Keys::K_OEM_PERIOD: return "OEM Period (. >)";
  case Keys::K_OEM_2: return "OEM 2 (/ ?)";
  case Keys::K_OEM_3: return "OEM 3 (` ~)";
  case Keys::K_ABNT_C1: return "Abnt C1";
  case Keys::K_ABNT_C2: return "Abnt C2";
  case Keys::K_OEM_4: return "OEM 4 ([ {)";
  case Keys::K_OEM_5: return "OEM 5 (\\ |)";
  case Keys::K_OEM_6: return "OEM 6 (] })";
  case Keys::K_OEM_7: return "OEM 7 (' @)";
  case Keys::K_OEM_8: return "OEM 8 (! §)";
  case Keys::K_OEM_AX: return "AX";
  case Keys::K_OEM_102: return "OEM 102 (< >)";
  case Keys::K_ICO_HELP: return "Ico Help";
  case Keys::K_ICO_00: return "Ico 00";
  case Keys::K_PROCESS: return "Process";
  case Keys::K_ICO_CLEAR: return "Ico Clear";
  case Keys::K_PACKET: return "Unicode Packet";
  case Keys::K_OEM_RESET: return "Reset";
  case Keys::K_OEM_JUMP: return "Jump";
  case Keys::K_OEM_PA1: return "OEM Pa1";
  case Keys::K_OEM_PA2: return "OEM Pa2";
  case Keys::K_OEM_PA3: return "OEM Pa3";
  case Keys::K_OEM_WSCTRL: return "WsCtrl";
  case Keys::K_OEM_CUSEL: return "Cu Sel";
  case Keys::K_OEM_ATTN: return "Oem Secure Attention";
  case Keys::K_OEM_FINISH: return "Finish";
  case Keys::K_OEM_COPY: return "Copy";
  case Keys::K_OEM_AUTO: return "Auto";
  case Keys::K_OEM_ENLW: return "Enlw";
  case Keys::K_OEM_BACKTAB: return "Back Tab";
  case Keys::K_ATTN: return "Secure Attention"; 
  case Keys::K_CRSEL: return "Cr Sel";
  case Keys::K_EXSEL: return "Ex Sel";
  case Keys::K_EREOF: return "Erase EOF";
  case Keys::K_PLAY: return "Play";
  case Keys::K_ZOOM: return "Zoom";
  case Keys::K_NONAME: return "NoName";
  case Keys::K_PA1: return "PA1";
  case Keys::K_OEM_CLEAR: return "OEM Clr";
  default: return "OEM Special";
  }
}

} // namespace shinybear