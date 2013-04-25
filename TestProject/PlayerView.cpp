#include "view\RenderView.h"
#include "PlayerView.h"
#include "base\system\GraphicsProvider.h"
#include "view\input\InputManager.h"

PlayerView::PlayerView(shinybear::GraphicsProvider *provider)
  :RenderView(provider)
{
}

PlayerView::~PlayerView()
{
}