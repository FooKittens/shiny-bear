#include "DXApp.h"
#include "DXWindow.h"
#include "InputManager.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <cassert>
#include <cstdio>
#include <util\SBUtil.h>


#define COMRELEASE(X) if(X) X->Release()

namespace framework {

DXApp::DXApp(HINSTANCE hInstance, const SIZE &size)
  : DXWindow(hInstance, size), graphicsProvider(GetWindowHandle()) {
  
  m_displayFps = true;
  m_pFont = nullptr;
}

DXApp::~DXApp() {
  COMRELEASE(m_pFont);
}

void DXApp::Initialize() {
  
  // Initialize Keyboard manager.
  InputManager::Initialize();

  graphicsProvider.Initialize();

  shinybear::DisplayMode dpm;
  dpm.fullscreen = false;
  dpm.height = GetSize().cy;
  dpm.width = GetSize().cx;
  dpm.refreshRate = 60;

  shinybear::MultiSampleMode msaa;
  msaa.quality = 0;
  msaa.samples = 1;

  graphicsProvider.SetDisplayMode(dpm);
  graphicsProvider.SetMultiSampleMode(msaa);
  graphicsProvider.ApplyChanges();

  OnDeviceLost();
}

void DXApp::OnDeviceLost() {
  graphicsProvider.ResetDevice();

  //D3DXFONT_DESC fontDesc;
  //fontDesc.CharSet = DEFAULT_CHARSET;
  //fontDesc.Height = 22;
  //fontDesc.Width = 14;
  //fontDesc.Italic = false;
  //fontDesc.MipLevels = 0;
  //fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
  //fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
  //fontDesc.Quality = DEFAULT_QUALITY;
  //fontDesc.Weight = FW_BOLD;
  //strcpy(fontDesc.FaceName, "Consolas");

  //ReleaseCOM(m_pFont);
  //HRESULT hr = D3DXCreateFontIndirect(graphicsProvider.GetDevice(), &fontDesc, &m_pFont);
  //assert(SUCCEEDED(hr) && "Failed to create FPS font.");
}

void DXApp::Run() {
  m_isRunning = true;

  DXWindow::ShowWindow();
 
  m_gameTimer.Start();
  m_fpsTimer = m_frameCount = m_lastFrameCount = 0;

  while(m_isRunning) {
    // Handle Win32 messages.
    HandleMessages();

    // Advance the timer.
    m_gameTimer.Tick();
    Update(m_gameTimer.GetElapsedTime());

    if(graphicsProvider.IsDeviceLost()) {
      graphicsProvider.ResetDevice();
      OnDeviceLost();
    }

    DrawScene();

    if(m_displayFps) {
      m_fpsTimer += m_gameTimer.GetElapsedTime();
      m_frameCount += 1.0;
      
      if(m_fpsTimer >= 1.0) {
        m_lastFrameCount = m_frameCount;
        m_frameCount = m_fpsTimer = 0.0;
      }
    }
  }
}

void DXApp::Update(double deltaTime) {

}

void DXApp::DrawScene() {

}

void DXApp::DrawFpsText() {
  char buffer[20];

  double fps = m_frameCount + m_lastFrameCount * (1.0 - m_fpsTimer);
  sprintf(buffer, "FPS: %.2f", fps); 

  RECT fontrect = { 20, 20, 200, 120 };

  m_pFont->DrawTextA(NULL, buffer, strlen(buffer), &fontrect, DT_LEFT, 0xFFFFFFFF);

  sprintf(buffer, "TPF: %.10f", 1.0 / fps);
  fontrect.top += 20;
  m_pFont->DrawTextA(NULL, buffer, strlen(buffer), &fontrect, DT_LEFT, 0xFFFFFFFF);
}

LRESULT CALLBACK DXApp::WindowProc(HWND hwnd, UINT msg,
  WPARAM wparam, LPARAM lparam) {
  switch(msg) {
  case WM_DESTROY:
    m_isRunning = false;
    return 0;
  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}



} // namespace framework


