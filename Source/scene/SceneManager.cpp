#include "scene\SceneManager.h"
#include "scene\SceneNode.h"
#include "scene\SceneView.h"
#include "util\SBUtil.h"

namespace shinybear
{

SceneManager::SceneManager(GraphicsProvider *pProvider)
{
  D3DXCreateMatrixStack(0, &m_pMatStack);
  m_pRoot = DBG_NEW SceneNode();
  m_pView = DBG_NEW SceneView(pProvider, this);
}

SceneManager::~SceneManager()
{
 delete m_pRoot;
 RELEASECOM(m_pMatStack);
 delete m_pView;
}

void SceneManager::Update(double elapsedSeconds)
{
  m_pRoot->Update(elapsedSeconds);
}

void SceneManager::Render()
{
  m_pRoot->PreRender(this);
  m_pRoot->Render(this);
  m_pRoot->PostRender(this);

  m_pView->Render(m_renderList);
  m_renderList.clear();
}

void SceneManager::OnDeviceLost()
{
  m_pRoot->OnDeviceLost();
}

void SceneManager::OnDeviceReset()
{
  m_pRoot->OnDeviceReset();
}


} // namespace shinybear