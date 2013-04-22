#include "view\RenderView.h"
#include "events\EventManager.h"
#include "events\eventtypes\WindowEvents.h"
#include "events\eventtypes\GraphicsProviderEvents.h"

namespace shinybear {

RenderView::RenderView(GraphicsProvider *pProvider)
  :m_composer(pProvider)
{
  m_pProvider = pProvider;
  EventManager::RegisterEventListener(FocusChangedEvent::kEventType, this);
  EventManager::RegisterEventListener(DeviceLostEvent::kEventType, this);
  EventManager::RegisterEventListener(DeviceResetEvent::kEventType, this);
}


void RenderView::Update()
{
  if(m_doRender)
  {
    m_pProvider->Clear(Color4f(0, 0, 0, 0));
  
    m_pProvider->Present();
  }
}

void RenderView::StopRendering(bool val) {
  m_doRender = !val;
}

bool RenderView::HandleEvent(const EventPtr &evt)
{

  if(evt->GetType() == FocusChangedEvent::kEventType)
  {
    FocusChangedEvent *pEvent = dynamic_cast<FocusChangedEvent*>(evt.get());
    if(pEvent->GetNewState() == FocusState::FS_LOSTFOCUS && 
       m_pProvider->IsFullscreen())
    {
      StopRendering(true);
    }
    else if(pEvent->GetNewState() == FocusState::FS_GAINEDFOCUS) {
      StopRendering(false);
    }
  }

  return false;
}



} // namespace shinybear