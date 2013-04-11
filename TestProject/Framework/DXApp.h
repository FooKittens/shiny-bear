#ifndef DXAPP_H
#define DXAPP_H

#include "GameTimer.h"
#include "DXWindow.h"
#include <Windows.h>
#include <graphics\DXGraphicsProvider.h>

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
  inline virtual shinybear::DXGraphicsProvider& GetProvider();
  inline virtual const GameTimer& GetTimer() { return m_gameTimer; }
  inline virtual void SetDisplayFps(bool val) { m_displayFps = val; }
  
  virtual void OnDeviceLost();

private:
  bool m_isRunning;
  bool m_displayFps;
  double m_frameCount, m_lastFrameCount, m_fpsTimer;
  GameTimer m_gameTimer;
  ID3DXFont *m_pFont;
  shinybear::DXGraphicsProvider graphicsProvider;
};

inline shinybear::DXGraphicsProvider& DXApp::GetProvider() {
  return graphicsProvider;
}

} // namespace framework

#endif