#include "base\BaseGame.h"
#include "base\system\GameTimer.h"
#include "base\system\GameWindow.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"
#include "util\input\InputManager.h"
#include <fstream>



namespace shinybear
{

// Global const string for the expected config file name
const wchar_t *g_kConfigFileName = L"Engine.cfg";

BaseGame::BaseGame()
{
  GetAbsolutePath(g_kConfigFileName, &m_pConfigPath);
  m_pDiagFont = nullptr;
}

BaseGame::~BaseGame()
{
  SAFEDELETE(m_pGameTimer);
  SAFEDELETE(m_pGraphicsProvider);
  SAFEDELETE(m_pGameWindow);
  RELEASECOM(m_pDiagFont);
  delete[] m_pConfigPath;
}

void BaseGame::SaveConfig(const Config &cfg) const
{
  std::wfstream stream;
  stream.open(m_pConfigPath, std::ios::trunc | std::ios::out);
  cfg.ToText(stream);
  stream.close();
}

void BaseGame::LoadConfig(Config *pConfig) const
{
 
  size_t configPathLen = wcslen(m_pConfigPath);

  // If the config file does not exist, we save a default one.
  // TODO : Should calculate a more appropriate resolution.
  if(!FileExists(m_pConfigPath, configPathLen))
  {
    SaveConfig(Config::GetDefault());
  }
  
  std::wfstream stream;
  stream.open(m_pConfigPath, std::ios::in);
  pConfig->FromText(stream);
  stream.close();
}

// Loads the config file and sets up the initial gamestate.
bool BaseGame::Initialize()
{
  Config cfg = Config::GetDefault();
  LoadConfig(&cfg);

  // Create the gametimer.
  m_pGameTimer = DBG_NEW GameTimer();

  // Create our gamewindow.
  Size size;
  size.width = cfg.displayMode.width;
  size.height = cfg.displayMode.height;
  m_pGameWindow = DBG_NEW GameWindow(this, size);

  // Initialize input manager.
  InputManager::Initialize(*m_pGameWindow);

  // Register for window events.

  // Create the directx wrapper.
  m_pGraphicsProvider = DBG_NEW GraphicsProvider(m_pGameWindow->GetWindowHandle());
  if(m_pGraphicsProvider->Initialize())
  {  
    m_pGraphicsProvider->SetDisplayMode(cfg.displayMode);
    m_pGraphicsProvider->SetMultiSampleMode(cfg.multiSampleMode);
    m_pGraphicsProvider->ApplyChanges();

    OnDeviceReset();
  }
  else
  {
    // TODO: Handle GraphicsProvider errors.
    return false;
  }

  // Call derived method.
  if(!OnInitialize())
  {
    // TODO: Handle customized application init failure.
    return false;
  }

  return true;
}

// This method is called to start up the application.
// It initializes all subsystems, and then calls
// OnInitialize in the derived class after which it will then
// begin the game loop.
bool BaseGame::Run()
{

  if(!Initialize())
  {
    // TODO : Present useful information to user about initialization failure.
    return false;
  }

  m_pGameTimer->Start();
  m_pGameWindow->Show();
  
  Resume();

  m_fpsTimer = 0.0f;
  m_currentFrame = 0;
  m_lastFps = 0;

  m_isRunning   = true;
  m_isQuitting  = false;
  m_doRenderDiagnostics = true;

  while(m_isRunning)
  {
    // Handle Windows Messages.
    m_pGameWindow->HandleMessages();

    // Don't tick the timer if we're paused.
    if(!m_isPaused)
    {
      m_pGameTimer->Tick();
    }

    // Update gamelogic.
    OnUpdate(m_pGameTimer->GetElapsedTime());

    // Check the state of the graphicsprovider.
    // This will reset the device if necessary.
    if(CheckDeviceState() == S_OK)
    {
      HR(m_pGraphicsProvider->GetDevice()->BeginScene());
      // call virtual method to delegate rendering to derived classes.
      OnRender();
      if(m_doRenderDiagnostics)
      {
        RenderDiagnostics(m_pDiagFont);
      }

      HR(m_pGraphicsProvider->GetDevice()->EndScene());

      m_pGraphicsProvider->Present();
    }

    //Sleep(5);

    if(m_isQuitting)
    {
      // TODO : Prepare for exit..

      m_isRunning = false;
    }
  }

  return true;
}

void BaseGame::OnUpdate(double elapsedSeconds)
{
  ++m_currentFrame;
  m_fpsTimer += m_pGameTimer->GetElapsedTime();
  if(m_fpsTimer > 1.0f)
  {
    m_lastFps = m_currentFrame;
    m_currentFrame = 0;
    m_fpsTimer = 0;
  }
}

void BaseGame::RenderDiagnostics(ID3DXFont *pFont)
{
  char buffer[512];
  sprintf(buffer, "FPS: %.2f", GetCurrentFps());
  RECT drawRect = { 20, 20, 200, 150 };
  pFont->DrawTextA(NULL, buffer, strlen(buffer),
    &drawRect, 0 , 0xFFFFFFFF);
}

HRESULT BaseGame::CheckDeviceState()
{
  if(!m_hasFocus && m_pGraphicsProvider->IsFullscreen())
  {
    return D3DERR_DEVICENOTRESET;
  }
  HRESULT hr = m_pGraphicsProvider->GetDeviceState();
  if(hr == D3DERR_DEVICENOTRESET)
  {
    OnDeviceLost();
    hr = m_pGraphicsProvider->ResetDevice();
    if(hr == S_OK)
    {
      OnDeviceReset();
    }
  }
  //else if(hr == D3DERR_DEVICELOST && m_hasFocus)
  //{
  //  OnDeviceLost();
  //  m_pGraphicsProvider->ApplyChanges();
  //  OnDeviceReset();
  //}

  return hr;
}

void BaseGame::Exit()
{
  m_isQuitting = true;
}

void BaseGame::Pause()
{
  m_isPaused = true;
  m_pGameTimer->Pause();
}

void BaseGame::Resume()
{
  m_isPaused = false;
  m_pGameTimer->Resume();
}

void BaseGame::OnWindowClosed()
{
  m_isQuitting = true;
}

void BaseGame::OnFocusChanged(bool getFocus)
{
  m_hasFocus = getFocus;
  if(getFocus == true)
  {
    if(m_pGraphicsProvider->IsFullscreen())
    {
      Sleep(50);
    }
    CheckDeviceState();
  }
}

void BaseGame::OnDeviceLost()
{
  RELEASECOM(m_pDiagFont);
}

void BaseGame::OnDeviceReset()
{
  D3DXFONT_DESC fontdesc;
  fontdesc.CharSet = DEFAULT_CHARSET;
  fontdesc.Height = 22;
  fontdesc.Italic = false;
  fontdesc.MipLevels = 0;
  fontdesc.OutputPrecision = OUT_TT_ONLY_PRECIS;
  fontdesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
  fontdesc.Quality = DEFAULT_QUALITY;
  fontdesc.Weight = FW_NORMAL;
  fontdesc.Width = 0;
  strcpy(fontdesc.FaceName, "Arial");
  HR(D3DXCreateFontIndirect(m_pGraphicsProvider->GetDevice(), &fontdesc, &m_pDiagFont));
}

} // namespace shinybear