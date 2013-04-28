#include "scene\SceneManager.h"
#include "scene\SceneNode.h"
#include "util\SBUtil.h"

namespace shinybear
{

SceneManager::SceneManager(GraphicsProvider *pProvider)
{
  D3DXCreateMatrixStack(0, &m_pMatStack);
  m_pRoot = new SceneNode();
}

SceneManager::~SceneManager()
{
 delete m_pRoot;
 RELEASECOM(m_pMatStack);
}

void SceneManager::Update(double elapsedSeconds)
{
  m_pRoot->Update(elapsedSeconds);
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