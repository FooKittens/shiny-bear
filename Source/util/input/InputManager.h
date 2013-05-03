#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "base\system\GameTimer.h"
#include "util\math\Math.h"

#include <windows.h>
#include <bitset>
#include <string>
#include <cassert>
#include <climits>
#include <XInput.h>

namespace shinybear { class BaseGame; class GameWindow; }

namespace shinybear
{

#pragma region Keyboard
namespace Keys
{
enum Enum
{
  // 0x01 , 0x02 Mouse buttons
  K_CONTROLBREAK = VK_CANCEL, // 0x03
  // 0x04 - 0x06 Mouse buttons
  // 0x07 Undefined
  K_BACKSPACE = VK_BACK, // 0x08
  K_TAB = VK_TAB, // 0x09
  // 0x0A , 0x0B Undefined
  K_CLEAR = VK_CLEAR, // 0x0C
  K_ENTER = VK_RETURN, // 0x0D
  // 0x0E , 0x0F Undefined
  K_SHIFT = VK_SHIFT, // 0x10
  K_CTRL = VK_CONTROL, // 0x11
  K_ALT = VK_MENU, // 0x12
  K_PAUSE = VK_PAUSE, // 0x13
  K_CAPSLOCK = VK_CAPITAL, // 0x14
  K_KANA = VK_KANA, // 0x15
  // 0x16 Reserved for Kanji systems
  K_JUNJA = VK_JUNJA, // 0x17
  K_FINAL = VK_FINAL, // 0x18
  K_KANJI = VK_KANJI, // 0x19
  // 0x1A undefined
  K_ESCAPE = VK_ESCAPE, // 0x1B
  K_CONVERT = VK_CONVERT, // 0x1C
  K_NONCONVERT = VK_NONCONVERT, // 0x1D
  K_ACCEPT = VK_ACCEPT, // 0x1E
  K_MODECHANGE = VK_MODECHANGE, // 0x1F
  K_SPACEBAR = VK_SPACE, // 0x20
  K_PAGEUP = VK_PRIOR, // 0x21
  K_PAGEDOWN = VK_NEXT, // 0x22
  K_END = VK_END, // 0x23
  K_HOME = VK_HOME, // 0x24
  K_LEFT = VK_LEFT, // 0x25
  K_UP = VK_UP, // 0x26
  K_RIGHT = VK_RIGHT, // 0x27
  K_DOWN = VK_DOWN, // 0x28
  K_SELECT = VK_SELECT, // 0x29
  K_PRINT = VK_PRINT, // 0x2A
  K_EXECUTE = VK_EXECUTE, // 0x2B
  K_PRINTSCREEN = VK_SNAPSHOT, // 0x2C
  K_INSERT = VK_INSERT, // 0x2D
  K_DELETE = VK_DELETE, // 0x2E
  K_HELP = VK_HELP, // 0x2F
  K_0 = '0', // 0x30
  K_1 = '1', // 0x31
  K_2 = '2', // 0x32
  K_3 = '3', // 0x33
  K_4 = '4', // 0x34
  K_5 = '5', // 0x35
  K_6 = '6', // 0x36
  K_7 = '7', // 0x37
  K_8 = '8', // 0x38
  K_9 = '9', // 0x39
  // 0x3A - 0x40 Undefined
  K_A = 'A', // 0x41
  K_B = 'B', // 0x42
  K_C = 'C', // 0x43
  K_D = 'D', // 0x44
  K_E = 'E', // 0x45
  K_F = 'F', // 0x46
  K_G = 'G', // 0x47
  K_H = 'H', // 0x48
  K_I = 'I', // 0x49
  K_J = 'J', // 0x4A
  K_K = 'K', // 0x4B
  K_L = 'L', // 0x4C
  K_M = 'M', // 0x4D
  K_N = 'N', // 0x4E
  K_O = 'O', // 0x4F
  K_P = 'P', // 0x50
  K_Q = 'Q', // 0x51
  K_R = 'R', // 0x52
  K_S = 'S', // 0x53
  K_T = 'T', // 0x54
  K_U = 'U', // 0x55
  K_V = 'V', // 0x56
  K_W = 'W', // 0x57
  K_X = 'X', // 0x58
  K_Y = 'Y', // 0x59
  K_Z = 'Z', // 0x5A
  K_LEFTWINDOWS = VK_LWIN, // 0x5B
  K_RIGHTWINDOWS = VK_RWIN, // 0x5C
  K_APPLICATIONS = VK_APPS, // 0x5D
  // 0x5E Undefined
  K_SLEEP = VK_SLEEP, // 0x5F
  K_NUMPAD0 = VK_NUMPAD0, // 0x60
  K_NUMPAD1 = VK_NUMPAD1, // 0x61
  K_NUMPAD2 = VK_NUMPAD2, // 0x62
  K_NUMPAD3 = VK_NUMPAD3, // 0x63
  K_NUMPAD4 = VK_NUMPAD4, // 0x64
  K_NUMPAD5 = VK_NUMPAD5, // 0x65
  K_NUMPAD6 = VK_NUMPAD6, // 0x66
  K_NUMPAD7 = VK_NUMPAD7, // 0x67
  K_NUMPAD8 = VK_NUMPAD8, // 0x68
  K_NUMPAD9 = VK_NUMPAD9, // 0x69
  K_MULTIPLY = VK_MULTIPLY, // 0x6A
  K_ADD = VK_ADD, // 0x6B
  K_SEPARATOR = VK_SEPARATOR, // 0x6C
  K_SUBTRACT = VK_SUBTRACT, // 0x6D
  K_DECIMAL = VK_DECIMAL, // 0x6E
  K_DIVIDE = VK_DIVIDE, // 0x6F
  K_F1 = VK_F1, // 0x70
  K_F2 = VK_F2, // 0x71
  K_F3 = VK_F3, // 0x72
  K_F4 = VK_F4, // 0x73
  K_F5 = VK_F5, // 0x74
  K_F6 = VK_F6, // 0x75
  K_F7 = VK_F7, // 0x76
  K_F8 = VK_F8, // 0x77
  K_F9 = VK_F9, // 0x78
  K_F10 = VK_F10, // 0x79
  K_F11 = VK_F11, // 0x7A
  K_F12 = VK_F12, // 0x7B
  K_F13 = VK_F13, // 0x7C
  K_F14 = VK_F14, // 0x7D
  K_F15 = VK_F15, // 0x7E
  K_F16 = VK_F16, // 0x7F
  K_F17 = VK_F17, // 0x80
  K_F18 = VK_F18, // 0x81
  K_F19 = VK_F19, // 0x82
  K_F20 = VK_F20, // 0x83
  K_F21 = VK_F21, // 0x84
  K_F22 = VK_F22, // 0x85
  K_F23 = VK_F23, // 0x86
  K_F24 = VK_F24, // 0x87
  // 0x88 - 0x8F Unassigned
  K_NUMLOCK = VK_NUMLOCK, // 0x90
  K_SCROLLLOCK = VK_SCROLL, // 0x91
  K_JISHO = VK_OEM_FJ_JISHO, // 0x92
  K_MASSHOU = VK_OEM_FJ_MASSHOU, // 0x93
  K_TOUROKU = VK_OEM_FJ_TOUROKU, // 0x94
  K_LOYA = VK_OEM_FJ_LOYA, // 0x95
  K_ROYA = VK_OEM_FJ_ROYA, // 0x96
  // 0x97 - 0x9F ???
  K_LEFTSHIFT = VK_LSHIFT, // 0xA0
  K_RIGHTSHIFT = VK_RSHIFT, // 0xA1
  K_LEFTCTRL = VK_LCONTROL, // 0xA2
  K_RIGHTCTRL = VK_RCONTROL, // 0xA3
  K_LEFTALT = VK_LMENU, // 0xA4
  K_RIGHTALT = VK_RMENU, // 0xA5
  K_BROWSER_BACK = VK_BROWSER_BACK, // 0xA6
  K_BROWSER_FORWARD = VK_BROWSER_FORWARD, // 0xA7
  K_BROWSER_REFRESH = VK_BROWSER_REFRESH, // 0xA8
  K_BROWSER_STOP = VK_BROWSER_STOP, // 0xA9
  K_BROWSER_SEARCH = VK_BROWSER_SEARCH, // 0xAA
  K_BROWSER_FAVORITES = VK_BROWSER_FAVORITES, // 0xAB
  K_BROWSER_HOME = VK_BROWSER_HOME, // 0xAC
  K_VOLUME_MUTE = VK_VOLUME_MUTE, // 0xAD
  K_VOLUME_DOWN = VK_VOLUME_DOWN, // 0xAE
  K_VOLUME_UP = VK_VOLUME_UP, // 0xAF
  K_MEDIA_NEXT_TRACK = VK_MEDIA_NEXT_TRACK, // 0xB0
  K_MEDIA_PREV_TRACK = VK_MEDIA_PREV_TRACK, // 0xB1
  K_MEDIA_STOP = VK_MEDIA_STOP, // 0xB2
  K_MEDIA_PLAY_PAUSE = VK_MEDIA_PLAY_PAUSE, // 0xB3
  K_LAUNCH_MAIL = VK_LAUNCH_MAIL, // 0xB4
  K_LAUNCH_MEDIA_SELECT = VK_LAUNCH_MEDIA_SELECT, // 0xB5
  K_LAUNCH_APP1 = VK_LAUNCH_APP1, // 0xB6
  K_LAUNCH_APP2 = VK_LAUNCH_APP2, // 0xB7
  // 0xB8 - 0xB9 ???
  K_OEM_1 = VK_OEM_1, // 0xBA
  K_OEM_PLUS = VK_OEM_PLUS, // 0xBB
  K_OEM_COMMA = VK_OEM_COMMA, // 0xBC
  K_OEM_MINUS = VK_OEM_MINUS, // 0xBD
  K_OEM_PERIOD = VK_OEM_PERIOD, // 0xBE
  K_OEM_2 = VK_OEM_2, // 0xBF
  K_OEM_3 = VK_OEM_3, // 0xC0
  K_ABNT_C1 = 0xC1, // 0xC1
  K_ABNT_C2 = 0xC2, // 0xC2
  // 0xC3 - 0xDA Unassigned
  K_OEM_4 = VK_OEM_4, // 0xDB
  K_OEM_5 = VK_OEM_5, // 0xDC
  K_OEM_6 = VK_OEM_6, // 0xDD
  K_OEM_7 = VK_OEM_7, // 0xDE
  K_OEM_8 = VK_OEM_8, // 0xDF
  K_OEM_AX = VK_OEM_AX, // 0xE1
  K_OEM_102 = VK_OEM_102, // 0xE2
  K_ICO_HELP = VK_ICO_HELP, // 0xE3
  K_ICO_00 = VK_ICO_00, // 0xE4
  K_PROCESS = VK_PROCESSKEY, // 0xE5
  K_ICO_CLEAR = VK_ICO_CLEAR, // 0xE6
  K_PACKET = VK_PACKET, // 0xE7
  /* 0xE7 is used to pass Unicode characters as if they were keystrokes.
   * If VK_PACKET is used with SendInput, then the Unicode character to be delivered
   * should be placed into the lower 16 bits of the scan code. If a keyboard message
   * is removed from the message queue and the virtual key is VK_PACKET, then the Unicode
   * character will be the upper 16 bits of the lparam. */
  // 0xE8 Unassigned
  K_OEM_RESET = VK_OEM_RESET, // 0xE9
  K_OEM_JUMP = VK_OEM_JUMP, // 0xEA
  K_OEM_PA1 = VK_OEM_PA1, // 0xEB
  K_OEM_PA2 = VK_OEM_PA2, // 0xEC
  K_OEM_PA3 = VK_OEM_PA3, // 0xED
  K_OEM_WSCTRL = VK_OEM_WSCTRL, // 0xEE
  K_OEM_CUSEL = VK_OEM_CUSEL, // 0xEF
  K_OEM_ATTN = VK_OEM_ATTN, // 0xF0
  K_OEM_FINISH = VK_OEM_FINISH, // 0xF1
  K_OEM_COPY = VK_OEM_COPY, // 0xF2
  K_OEM_AUTO = VK_OEM_AUTO, // 0xF3
  K_OEM_ENLW = VK_OEM_ENLW, // 0xF4
  K_OEM_BACKTAB = VK_OEM_BACKTAB, // 0xF5
  K_ATTN = VK_ATTN, // 0xF6
  K_CRSEL = VK_CRSEL, // 0xF7
  K_EXSEL = VK_EXSEL, // 0xF8
  K_EREOF = VK_EREOF, // 0xF9
  K_PLAY = VK_PLAY, // 0xFA
  K_ZOOM = VK_ZOOM, // 0xFB
  K_NONAME = VK_NONAME, // 0xFC Reserved for future use
  K_PA1 = VK_PA1, // 0xFD
  K_OEM_CLEAR = VK_OEM_CLEAR, // 0xFE
};
}

struct KeyboardState
{
public:
  friend class InputManager;

  //ctors
  KeyboardState(){}
  KeyboardState(const KeyboardState &keyState)
  {
    keyStates = keyState.keyStates;
  }

  //key checks
  bool const IsKeyDown(const Keys::Enum key) const
  {
    return !!(keyStates[key - 1] & 1);
  }

  bool const IsAnyKeyDown() const
  {
    for(int i = 0; i != 255; ++i)
    {
      if(!!(keyStates[i] & 1))
      {
        return true;
      }
    }
    return false;
  }

  //statics
  static const std::string GetKeyName(const Keys::Enum key)
  {
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

  // operators
  void operator=(const KeyboardState &keyState)
  {
    keyStates = keyState.keyStates;
  }
private:
  // number 255 is used for fake keys, therefore
  // size is not 256; The range is 0-254.
  std::bitset<255> keyStates;
};
#pragma endregion

#pragma region Mouse
namespace MouseButtons
{
enum Enum
{
  MB_LEFTMOUSE = VK_LBUTTON, // 0x01
  MB_RIGHTMOUSE = VK_RBUTTON, // 0x02
  MB_MIDDLEMOUSE = VK_MBUTTON, // 0x04
  MB_XBUTTON1 = VK_XBUTTON1, // 0x05
  MB_XBUTTON2 = VK_XBUTTON2, // 0x06
};
}

struct MouseState
{
public:
  friend class InputManager;

  //ctors
  MouseState(){}
  MouseState(const MouseState &mouseState)
  {
    mouseButtonStates = mouseState.mouseButtonStates;
    mouseWheelChange = mouseState.mouseWheelChange;
    absoluteX = mouseState.absoluteX;
    absoluteY = mouseState.absoluteY;
    changeX = mouseState.changeX;
    changeY = mouseState.changeY;
  }

  //mouse checks
  const bool IsButtonDown(const MouseButtons::Enum button) const
  {
    return !!(mouseButtonStates[button - 1] & 1);
  }

  //statics
  static const std::string GetButtonName(const MouseButtons::Enum button)
  {
    switch(button)
    {
    case MouseButtons::MB_LEFTMOUSE: return "Left Mouse";
    case MouseButtons::MB_RIGHTMOUSE: return "Right Mouse";
    case MouseButtons::MB_MIDDLEMOUSE: return "Middle Mouse";
    case MouseButtons::MB_XBUTTON1: return "X1 Mouse";
    case MouseButtons::MB_XBUTTON2: return "X2 Mouse";
    default: return "OEM Special";
    }
  }

  //operators
  void operator=(const MouseState &mouseState)
  {
    mouseButtonStates = mouseState.mouseButtonStates;
    mouseWheelChange = mouseState.mouseWheelChange;
    absoluteX = mouseState.absoluteX;
    absoluteY = mouseState.absoluteY;
    changeX = mouseState.changeX;
    changeY = mouseState.changeY;
  }

private:
  // size 6 to match Microsoft's VK_-defines, where 0x03 is a
  // non-mouse related VK, with 1,2,4,5,6 being mouse buttons
  std::bitset<6> mouseButtonStates;
  // Mouse wheel change
  short mouseWheelChange;
  // Absolute mouse position, according to windows
  int absoluteX, absoluteY;
  // Mouse XY change since previous event
  int changeX, changeY;
};
#pragma endregion

#pragma region GamePad
namespace GamePadIndex
{
  enum Enum
  {
    ONE = 0,
    TWO = 1,
    THREE = 2,
    FOUR = 3,
  };
}

namespace ControllerButtons
{
  enum Enum
  {
    DPAD_UP = 1,
    DPAD_DOWN = 2,
    DPAD_LEFT = 3,
    DPAD_RIGHT = 4,
    START = 5,
    BACK = 6,
    LEFT_THUMB = 7,
    RIGHT_THUMB = 8,
    LEFT_SHOULDER = 9,
    RIGHT_SHOULDER = 10,
    GAMEPAD_A = 11,
    GAMEPAD_B = 12,
    GAMEPAD_X = 13,
    GAMEPAD_Y = 14,
  };
}

struct ControllerState
{
public:
  friend class InputManager;
  
  //ctors
  ControllerState(const GamePadIndex::Enum index)
    : leftTrigger(mLTrigger), rightTrigger(mRTrigger),
      leftThumbstick(mLThumb), rightThumbstick(mRThumb),
      isConnected(mConnected)
  {
    m_isVibrating = false;
    mIndex = index;
    Update();
  }
  ControllerState(const ControllerState &controllerState)
    : leftTrigger(mLTrigger), rightTrigger(mRTrigger),
      leftThumbstick(mLThumb), rightThumbstick(mRThumb),
      isConnected(mConnected)
  {
    mLTrigger = controllerState.mLTrigger;
    mRTrigger = controllerState.mRTrigger;
    mLThumb = controllerState.mLThumb;
    mRThumb = controllerState.mRThumb;

    mState = controllerState.mState;
    m_isVibrating = false;
  }

  //dtors
  ~ControllerState()
  {
    //stop controller vibration
    Vibrate();
  }
  
  const bool IsButtonDown(const ControllerButtons::Enum button) const
  {
    switch(button)
    {
    case ControllerButtons::DPAD_UP:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ? true : false;
    case ControllerButtons::DPAD_DOWN:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? true : false;
    case ControllerButtons::DPAD_LEFT:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? true : false;
    case ControllerButtons::DPAD_RIGHT:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? true : false;
    case ControllerButtons::START:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_START ? true : false;
    case ControllerButtons::BACK:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ? true : false;
    case ControllerButtons::LEFT_THUMB:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ? true : false;
    case ControllerButtons::RIGHT_THUMB:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ? true : false;
    case ControllerButtons::LEFT_SHOULDER:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? true : false;
    case ControllerButtons::RIGHT_SHOULDER:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? true : false;
    case ControllerButtons::GAMEPAD_A:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_A ? true : false;
    case ControllerButtons::GAMEPAD_B:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_B ? true : false;
    case ControllerButtons::GAMEPAD_X:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_X ? true : false;
    case ControllerButtons::GAMEPAD_Y:
      return mState.Gamepad.wButtons & XINPUT_GAMEPAD_Y ? true : false;
    default:
      return false;
    }
  }
  void SetGamePadIndex(const GamePadIndex::Enum index)
  {
    mIndex = index;
  }
  
  const float &leftTrigger, &rightTrigger;
  const Vector2 &leftThumbstick, &rightThumbstick;
  const bool &isConnected;
  bool m_isVibrating;

  //takes float values 0.0f - 1.0f
  void Vibrate(const float leftMotor = 0, const float rightMotor = 0)
  {
    if(m_isVibrating == false)
    {
      if(abs(leftMotor) < FLT_EPSILON && abs(rightMotor) < FLT_EPSILON)
      {
        return;
      }
      else
      {
        m_isVibrating = true;
      }
    }
    else
    {
      if(abs(leftMotor) < FLT_EPSILON && abs(rightMotor) < FLT_EPSILON)
      {
        m_isVibrating = false;
      }
    }

    // Create a Vibraton State
    XINPUT_VIBRATION vibration;

    // Set the Vibration Values
    vibration.wLeftMotorSpeed = (unsigned short)(max(0.0f, min(leftMotor, 1.0f)) * USHRT_MAX);
    vibration.wRightMotorSpeed = (unsigned short)(max(0.0f, min(rightMotor, 1.0f)) * USHRT_MAX);

    // Vibrate the controller
    XInputSetState(mIndex, &vibration);
  }

  //statics
  static const std::string GetButtonName(const ControllerButtons::Enum button)
  {
    switch(button)
    {
    case ControllerButtons::DPAD_UP: return "Dpad Up";
    case ControllerButtons::DPAD_DOWN: return "Dpad Down";
    case ControllerButtons::DPAD_LEFT: return "Dpad Down";
    case ControllerButtons::DPAD_RIGHT: return "Dpad Right";
    case ControllerButtons::START: return "Start";
    case ControllerButtons::BACK: return "Back";
    case ControllerButtons::LEFT_THUMB: return "Left thumbstick";
    case ControllerButtons::RIGHT_THUMB: return "Right thumbstick";
    case ControllerButtons::LEFT_SHOULDER: return "Left shoulder";
    case ControllerButtons::RIGHT_SHOULDER: return "Right shoulder";
    case ControllerButtons::GAMEPAD_A: return "A";
    case ControllerButtons::GAMEPAD_B: return "B";
    case ControllerButtons::GAMEPAD_X: return "X";
    case ControllerButtons::GAMEPAD_Y: return "Y";
    }
  }

  //operators
  void operator=(const ControllerState &controllerState)
  {
    mLTrigger = controllerState.mLTrigger;
    mRTrigger = controllerState.mRTrigger;
    mLThumb = controllerState.mLThumb;
    mRThumb = controllerState.mRThumb;
    mState = controllerState.mState;
  }

private:
  // All 14 Xbox Controller buttons:
  // DPAD (4), start, back, thumbsticks (2), shoulders (2), ABXY (4)
  float mLTrigger, mRTrigger;
  Vector2 mLThumb, mRThumb;

  XINPUT_STATE mState;
  bool mConnected;
  
  GamePadIndex::Enum mIndex;

  void Update()
  {

    // Sets mState
    mConnected = XInputGetState(mIndex, &mState) == ERROR_SUCCESS;

    if(mConnected)
    {
      // Check to make sure we are not moving during the dead zone (LTHUMB)
      if(mState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	       mState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {    
	      mState.Gamepad.sThumbLX = 0;
      }
      if(mState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	       mState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {
        mState.Gamepad.sThumbLY = 0;
      }
      mLThumb.x = mState.Gamepad.sThumbLX / (float)SHRT_MAX;
      mLThumb.y = mState.Gamepad.sThumbLY / (float)SHRT_MAX;

      // Check to make sure we are not moving during the dead zone (RTHUMB)
      if(mState.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	       mState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {    
	      mState.Gamepad.sThumbRX = 0;
      }
      if(mState.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	       mState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {
        mState.Gamepad.sThumbRY = 0;
      }
      mRThumb.x = mState.Gamepad.sThumbRX / (float)SHRT_MAX;
      mRThumb.y = mState.Gamepad.sThumbRY / (float)SHRT_MAX;

      // Check to make sure we are not moving during the dead zone (LTRIGGER)
      if(mState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      {    
	      mState.Gamepad.bLeftTrigger = 0;
      }
      mLTrigger = mState.Gamepad.bLeftTrigger / (float)UCHAR_MAX;
      // Check to make sure we are not moving during the dead zone (RTRIGGER)
      if(mState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      {    
	      mState.Gamepad.bRightTrigger = 0;
      }
      mRTrigger = mState.Gamepad.bRightTrigger / (float)UCHAR_MAX;
    }
  }
};
#pragma endregion

class InputManager
{
public:
  friend class BaseGame;
  friend class GameWindow;

  static void GetKeyboardState(KeyboardState * const);
  static void GetMouseState(MouseState * const);
  static void GetControllerState(ControllerState * const);

private:
  static void Initialize(const GameWindow &pGameWindow);

  static void Update();

  static void HandleInput(const HRAWINPUT &);

  static MouseState mouseState;
  static KeyboardState keyboardState;
  static const int kMaximumControllerCount = 4;
  static ControllerState xboxControllers[kMaximumControllerCount];
  static std::bitset<255> tempKey;
};

// Inlines
inline void InputManager::GetKeyboardState(KeyboardState * const pState)
{
  assert(pState);
  *pState = keyboardState;
}

inline void InputManager::GetMouseState(MouseState * const pState)
{
  assert(pState);
  *pState = mouseState;
}

inline void InputManager::GetControllerState(ControllerState * const pState)
{
  assert(pState);
  *pState = xboxControllers[pState->mIndex];
}

} // namespace shinybear

#endif