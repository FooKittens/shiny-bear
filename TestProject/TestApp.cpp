#include "TestApp.h"
#include <view\RenderView.h>

using namespace shinybear;

TestApp::TestApp() 
{
  OnInitialize();
}

TestApp::~TestApp()
{
  delete m_pRenderView;
}

bool TestApp::OnInitialize() 
{

  m_pRenderView = new RenderView(GetGraphicsProvider());

  AttachView(m_pRenderView);
  return true;
}


void TestApp::OnUpdate(double elapsedSeconds) 
{

}


