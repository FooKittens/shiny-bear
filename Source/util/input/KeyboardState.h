#ifndef SHINYBEAR_KEYBOARDSTATE_H
#define SHINYBEAR_KEYBOARDSTATE_H

#include <string>
#include <bitset>
#include <Windows.h>

namespace shinybear
{

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
} // namespace Keys

struct KeyboardState
{
public:
  friend class InputManager;

  //ctors
  KeyboardState();
  KeyboardState(const KeyboardState &keyState);

  //key checks
  bool IsKeyDown(const Keys::Enum key) const;
  bool IsAnyKeyDown() const;

  //statics
  static const std::string GetKeyName(const Keys::Enum key);

  // operators
  void operator=(const KeyboardState &keyState);

private:
  // number 255 is used for fake keys, therefore
  // size is not 256; The range is 0-254.
  std::bitset<255> m_keyStates;
};

inline void KeyboardState::operator=(const KeyboardState &keyState)
{
  m_keyStates = keyState.m_keyStates;
}

} // namespace shinybear

#endif // SHINYBEAR_KEYBOARDSTATE_H