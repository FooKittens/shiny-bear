#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "view\RenderView.h"
#include "base\system\GraphicsProvider.h"
#include "view\input\InputManager.h"

class PlayerView : public shinybear::RenderView
{
public:
  PlayerView(shinybear::GraphicsProvider *provider);
  ~PlayerView();
protected:

private:

};

#endif