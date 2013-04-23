#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "view\RenderView.h"


class PlayerView : RenderView
{
public:
  PlayerView();
  virtual ~PlayerView();

protected:

private:
  void OnInitialize();

}

#endif