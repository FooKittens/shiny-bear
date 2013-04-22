#include "view\RenderView.h"
#include "events\EventManager.h"

namespace shinybear
{

RenderView::RenderView(GraphicsProvider *pProvider)
  :m_composer(pProvider)
{
  m_pProvider = pProvider;
}


void RenderView::Update()
{
  m_pProvider->Clear(Color4f(0, 0, 0, 0));
  
  m_pProvider->Present();
}

bool RenderView::HandleEvent(const EventPtr &evt)
{

  return false;
}



} // namespace shinybear