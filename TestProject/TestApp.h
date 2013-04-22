#ifndef TESTAPP_H
#define TESTAPP_H

#include <base\BaseGame.h>

#include <windows.h>
#include <d3dx9.h>

namespace shinybear { class RenderView; }

class TestApp : public shinybear::BaseGame
{
public:
  TestApp();
  virtual ~TestApp();

protected:
  bool OnInitialize();

  void OnUpdate(double elapsedSeconds);
private:
  shinybear::RenderView *m_pRenderView;
};

#endif