#ifndef TESTAPP_H
#define TESTAPP_H

#include <base\BaseGame.h>
#include <windows.h>
#include <d3dx9.h>


class TestApp : public shinybear::BaseGame
{
public:
  TestApp();
  ~TestApp();

protected:
  bool OnInitialize();

  void OnUpdate(double elapsedSeconds);
private:
};

#endif