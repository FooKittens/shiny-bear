#include "DXApp.h"
#include "DXWindow.h"
#include "InputManager.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <cassert>
#include <cstdio>


#define COMRELEASE(X) if(X) X->Release()

namespace framework {

DXApp::DXApp(HINSTANCE hInstance, const SIZE &size)
  : DXWindow(hInstance, size) {
  
  m_displayFps = true;
}

DXApp::~DXApp() {
  COMRELEASE(m_pD3DObject);
  COMRELEASE(m_pDevice);
  COMRELEASE(m_pFont);
}

void DXApp::Initialize() {
  
  // Create Direct3D object for getting information about the system.
  m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

  assert(m_pD3DObject && "Failed to create Direct3D9 object!");

  // Retrieve the default adapters current displaymode.
  // Useful for windowed mode.
  D3DDISPLAYMODE mode;
  m_pD3DObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

  HRESULT hr = m_pD3DObject->CheckDeviceType(
    D3DADAPTER_DEFAULT, // Default adapter
    D3DDEVTYPE_HAL,     // Hardware driver type.
    mode.Format,        // Displayformat
    mode.Format,        // Backbuffer format
    TRUE                // Windowed
  );

  assert(SUCCEEDED(hr) && "Unsupported Device-configuration!");

  // Get Hardware capabilities.
  D3DCAPS9 dCaps;
  hr = m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &dCaps);

  assert(SUCCEEDED(hr) && "Failed to get device capabilities!");

  DWORD flags = 0;
  // Check for hardware vertex processing.
  if(dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
  else
    flags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  // Check if the device can be a pure device
  if(dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
    flags |= D3DCREATE_PUREDEVICE;
  
  D3DPRESENT_PARAMETERS dpp;
  dpp.Flags = 0;
  dpp.BackBufferWidth = GetSize().cx;
  dpp.BackBufferHeight = GetSize().cy;
  dpp.BackBufferFormat = mode.Format;
  dpp.BackBufferCount = 1;
  dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
  dpp.MultiSampleQuality = 0;
  dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  dpp.hDeviceWindow = GetWindowHandle();
  dpp.Windowed = TRUE;
  dpp.EnableAutoDepthStencil = TRUE;
  dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
  dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
  dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

  // Create the device pointer.
  hr = m_pD3DObject->CreateDevice(
    D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL,
    GetWindowHandle(),
    flags,
    &dpp,
    &m_pDevice
  );

  assert(SUCCEEDED(hr) && "Failed to D3D9Device!");

  // Initialize Keyboard manager.
  InputManager::Initialize();

  D3DXFONT_DESC fontDesc;
  fontDesc.CharSet = DEFAULT_CHARSET;
  fontDesc.Height = 22;
  fontDesc.Width = 14;
  fontDesc.Italic = false;
  fontDesc.MipLevels = 0;
  fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
  fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
  fontDesc.Quality = DEFAULT_QUALITY;
  fontDesc.Weight = FW_BOLD;
  strcpy(fontDesc.FaceName, "Consolas");

  hr = D3DXCreateFontIndirect(m_pDevice, &fontDesc, &m_pFont);
  
  assert(SUCCEEDED(hr) && "Failed to create FPS font.");
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
  D3DCOLOR cc = 0x404577;
  m_pDevice->BeginScene();
  m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, cc, 0, 0);

  DrawFpsText();
  m_pDevice->EndScene();
  m_pDevice->Present(NULL, NULL, NULL, NULL);
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

  switch(msg)
  {
  case WM_DESTROY:
    m_isRunning = false;
    return 0;
  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
  
}



} // namespace framework


