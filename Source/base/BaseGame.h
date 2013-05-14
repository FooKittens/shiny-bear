#ifndef SHINYBEAR_BASEGAME_H
#define SHINYBEAR_BASEGAME_H

#include "resource\proxy\D3DXProxy.h"
#include "base\Config.h"

#include <d3dx9.h>

// Forward declarations
namespace shinybear
{
  class GameTimer; class GraphicsProvider;
  class GameWindow; class IGameView;
  class FocusChangedEvent; class ILogic;
  class SoundProvider;
}

namespace shinybear
{

class BaseGame
{
public:
  friend class GameWindow;
  friend class ResourceManager;

  BaseGame();
  virtual ~BaseGame();

  // Should be called from main to start the application
  bool Run();

protected:
  // Derivative games should overshadow this method,
  // and perform their initial initialization here.
  virtual bool OnInitialize() = 0;
  
  virtual void OnWindowClosed();
  virtual void OnFocusChanged(bool getFocus);
  virtual void OnDeviceLost();
  virtual void OnDeviceReset();
  
  virtual void OnUpdate(double) = 0;
  virtual void OnRender() = 0;
  virtual void RenderDiagnostics(ID3DXFont *pDiagFont);

  // Called to close application.
  virtual void Exit();
  
  virtual bool HasFocus() const; 

  // Retrieve the games window.
  virtual GameWindow *GetWindow() const;
  virtual GraphicsProvider *GetGraphicsProvider() const;
  virtual SoundProvider *GetSoundProvider() const;

  virtual float GetCurrentFps();

  virtual void Pause(); 
  virtual void Resume();
  virtual bool IsPaused() const;
  

private:
  // Loads and saves config structs.
  void LoadConfig(Config *pConfig) const;
  void SaveConfig(const Config &cfg) const;

  bool Initialize();

  HRESULT CheckDeviceState();

  GameTimer *m_pGameTimer;
  GraphicsProvider *m_pGraphicsProvider;
  SoundProvider *m_pSoundProvider;
  GameWindow *m_pGameWindow;
  bool m_isRunning;
  bool m_isQuitting;
  bool m_isPaused;
  bool m_hasFocus;
  bool m_doRenderDiagnostics;
  wchar_t *m_pConfigPath;
  float m_fpsTimer;
  float m_currentFrame;
  float m_lastFps;
  FontProxy m_font;
  ID3DXFont *m_pDiagFont;
};


// Inlines

inline GameWindow *BaseGame::GetWindow() const
{
  return m_pGameWindow;
}

inline GraphicsProvider *BaseGame::GetGraphicsProvider() const
{
  return m_pGraphicsProvider;
}

inline SoundProvider *BaseGame::GetSoundProvider() const
{
  return m_pSoundProvider;
}

inline bool BaseGame::IsPaused() const
{
  return m_isPaused;
}

inline bool BaseGame::HasFocus() const
{
  return m_hasFocus;
}

inline float BaseGame::GetCurrentFps()
{
  return static_cast<float>(m_currentFrame + m_lastFps * (1.0f - m_fpsTimer));
}



} // namespace shinybear

#endif // SHINYBEAR_BASEGAME_H