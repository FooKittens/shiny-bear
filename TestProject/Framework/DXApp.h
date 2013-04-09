#ifndef DXAPP_H
#define DXAPP_H

#include "GameTimer.h"
#include "DXWindow.h"
#include <Windows.h>

struct IDirect3D9;
struct IDirect3DDevice9;
struct ID3DXFont;

namespace framework {

class DXApp : public DXWindow {
public:
  DXApp(HINSTANCE hinstance, const SIZE &size);
  virtual ~DXApp();

  // Make sure to call this method in any overloads.
  virtual void Initialize();

  // Don't overload this
  virtual void Run();

protected:

  // Overload these methods in new applications.
  virtual void Update(double deltaTime);
  virtual void DrawScene();

  // Calling this will terminate the application.
  inline virtual void Exit() { m_isRunning = false; }

  virtual void DrawFpsText();

  // Overshadow DXWindow's WinProc
  LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

  // Used to retrieve some objects for drawing etc.
  inline virtual IDirect3D9* GetDirect3D9Object() { return m_pD3DObject; }
  inline virtual IDirect3DDevice9* GetDevice() { return m_pDevice; }
  inline virtual const GameTimer& GetTimer() { return m_gameTimer; }
  inline virtual void SetDisplayFps(bool val) { m_displayFps = val; }
  
private:
  bool m_isRunning;
  bool m_displayFps;
  double m_frameCount, m_lastFrameCount, m_fpsTimer;
  GameTimer m_gameTimer;
  IDirect3D9 *m_pD3DObject;
  IDirect3DDevice9 *m_pDevice;
  ID3DXFont *m_pFont;
  
};

} // namespace framework

#endif