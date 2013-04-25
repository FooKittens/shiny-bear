#ifndef SHINYBEAR_BASEGAME_H
#define SHINYBEAR_BASEGAME_H

#include "base\Config.h"
#include "events\IEventListener.h"

#include <list>

// Forward declarations
namespace shinybear
{
  class GameTimer; class GraphicsProvider;
  class GameWindow; class IGameView;
  class FocusChangedEvent; class ILogic;
}

namespace shinybear
{

class BaseGame : public IEventListener
{
public:
  BaseGame();
  virtual ~BaseGame();

  // Should be called from main to start the application
  bool Run();

  // For Listener interface.
  const char *GetName() const { return "BaseGame"; }
  bool HandleEvent(const EventPtr &evt);
protected:
  // Derivative games should overshadow this method,
  // and perform their initial initialization here.
  virtual bool OnInitialize() = 0;

  // Called to close application.
  virtual void Exit();

  // Attaches a gameview that should do work related to
  // presenting the gamestate to the user.
  virtual void AttachView(IGameView *pView);
  virtual void RemoveView(IGameView *pView);

  // Changes the current logic object.
  virtual void SetLogic(ILogic *pLogic);
  
  virtual bool HasFocus() const; 

  // Retrieve the games window.
  virtual GameWindow *GetWindow() const;
  virtual GraphicsProvider *GetGraphicsProvider() const;

  virtual void Pause(); 
  virtual void Resume();
  virtual bool IsPaused() const;

private:
  // Loads and saves config structs.
  void LoadConfig(Config *pConfig) const;
  void SaveConfig(const Config &cfg) const;

  bool Initialize();

  // Used to avoid cluttering the mainloop, updates all views.
  void UpdateViews();

  // EventHandlerFunctions
  bool HandleFocusEvent(FocusChangedEvent *pEvent);

  // Holds the views for the MVC type model of the engine.
  std::list<IGameView*> m_views;

  // Holds the logic/model for the MVC style.
  ILogic *m_pLogic;

  GameTimer *m_pGameTimer;
  GraphicsProvider *m_pGraphicsProvider;
  GameWindow *m_pGameWindow;
  bool m_isRunning;
  bool m_isQuitting;
  bool m_isPaused;
  bool m_hasFocus;
  wchar_t *m_pConfigPath;
  
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

inline bool BaseGame::IsPaused() const
{
  return m_isPaused;
}

inline bool BaseGame::HasFocus() const {
  return m_hasFocus;
}



} // namespace shinybear

#endif // SHINYBEAR_BASEGAME_H