#include "base\BaseGame.h"
#include "base\system\GameTimer.h"
#include "base\system\GameWindow.h"
#include "base\system\GraphicsProvider.h"
#include "events\EventManager.h"
#include "events\eventtypes\WindowEvents.h"
#include "events\eventtypes\GraphicsProviderEvents.h"
#include "util\SBUtil.h"
#include "view\IGameView.h"
#include "view\input\InputManager.h"
#include <fstream>



namespace shinybear
{

// Global const string for the expected config file name
const wchar_t *g_kConfigFileName = L"Engine.cfg";

BaseGame::BaseGame()
{
  GetAbsolutePath(g_kConfigFileName, &m_pConfigPath);
}

BaseGame::~BaseGame()
{
  SAFEDELETE(m_pGameTimer);
  SAFEDELETE(m_pGraphicsProvider);
  SAFEDELETE(m_pGameWindow);
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

  EventManager::Initialize();
  EventManager::RegisterEventType(EventType::kWildCard);

  // Create the gametimer.
  m_pGameTimer = DBG_NEW GameTimer();

  // Create our gamewindow.
  Size size;
  size.width = cfg.displayMode.width;
  size.height = cfg.displayMode.height;
  m_pGameWindow = DBG_NEW GameWindow(size);

  // Initialize input manager.
  InputManager::Initialize(*m_pGameWindow);

  // Register for window events.
  EventManager::RegisterEventListener(WindowClosedEvent::kEventType, this);
  EventManager::RegisterEventListener(SessionStateChangedEvent::kEventType, this);
  EventManager::RegisterEventListener(FocusChangedEvent::kEventType, this);

  // Create the directx wrapper.
  m_pGraphicsProvider = DBG_NEW GraphicsProvider(m_pGameWindow->GetWindowHandle());
  if(m_pGraphicsProvider->Initialize())
  {  

    m_pGraphicsProvider->SetDisplayMode(cfg.displayMode);
    m_pGraphicsProvider->SetMultiSampleMode(cfg.multiSampleMode);
    m_pGraphicsProvider->ApplyChanges();

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
  
  m_isRunning   = true;
  m_isQuitting  = false;
  while(m_isRunning)
  {
    // Handle Windows Messages.
    m_pGameWindow->HandleMessages();
    // Process events on the eventqueue.
    EventManager::ProcessEvents();

    // Don't tick the timer if we're paused.
    if(!m_isPaused)
    {
     m_pGameTimer->Tick();
    }
    // Call OnUpdate for derived class.
    OnUpdate(m_pGameTimer->GetElapsedTime());

    // Update all gameviews.
    UpdateViews();

    if(m_isQuitting)
    {
      // TODO : Prepare for exit..

      EventManager::CleanUp();

      m_isRunning = false;
    }
  }

  return true;
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

bool BaseGame::HandleEvent(const EventPtr &evt)
{

  if(evt->GetType() == WindowClosedEvent::kEventType)
  {
    Exit();
    return false;
  }
  else if(evt->GetType() == SessionStateChangedEvent::kEventType)
  {
    SessionStateChangedEvent *sEvent = dynamic_cast<SessionStateChangedEvent*>(evt.get());

    // Retrieve the new sessionstate.
    SessionState newState = sEvent->GetNewState();

    if(newState == SessionState::SS_LOCKED)
    {
      // The desktop has been locked. Which will cause the graphics device to be lost.
      EventManager::PushImmediateEvent(EventPtr(DBG_NEW DeviceLostEvent()));
    }
    else if(newState == SessionState::SS_UNLOCKED)
    {
      if(m_pGraphicsProvider->IsDeviceLost())
      {
        m_pGraphicsProvider->ApplyChanges();
        EventManager::PushImmediateEvent(EventPtr(DBG_NEW DeviceLostEvent()));
      }
    }

  } 
  else if(evt->GetType() == FocusChangedEvent::kEventType)
  {
    FocusChangedEvent *fEvent = dynamic_cast<FocusChangedEvent*>(evt.get());

    if(fEvent->GetNewState() == FocusState::FS_LOSTFOCUS)
    {

    }
    else if(fEvent->GetNewState() == FocusState::FS_GAINEDFOCUS)
    {
      if(m_pGraphicsProvider->IsDeviceLost())
      {
        EventManager::PushImmediateEvent(EventPtr(DBG_NEW DeviceLostEvent()));
        // Recreate the device.
        m_pGraphicsProvider->ApplyChanges();

        EventManager::PushImmediateEvent(EventPtr(DBG_NEW DeviceResetEvent()));
      }
    }
  }

  return false;
}

void BaseGame::AttachView(IGameView *pView)
{
  m_views.push_back(pView);
  OutputDbgFormat("View  [%s] attached.", pView->GetName());
}

void BaseGame::RemoveView(IGameView *pView)
{
  auto it = m_views.begin();
  auto end = m_views.end();
  while(it != end)
  {
    if(*it++ = pView) 
    {
      m_views.erase(it);
      break;
    }
  }

  OutputDbgFormat("View [%s] detached.", pView->GetName());
}

inline void BaseGame::UpdateViews()
{
  auto it = m_views.begin();
  auto end = m_views.end();
  while(it != end)
  {
    (*it++)->Update();
  }
}

} // namespace shinybear