#include "scene\SceneNode.h"
#include "scene\SceneManager.h"
#include <cassert>

namespace shinybear
{

SceneNode::SceneNode()
{
  m_pParent = nullptr;
  SetVisible(true);
  LoadIdentity();
}

SceneNode::~SceneNode()
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->SetParent(nullptr);
    delete m_children[i];
  }

  m_children.clear();
  m_children.shrink_to_fit();
}

void SceneNode::Attach(SceneNode *pNode)
{
  m_children.push_back(pNode);
  pNode->SetParent(this);
}

void SceneNode::Detach(SceneNode *pNode)
{
  auto it = m_children.begin();
  auto end = m_children.end();

  while(it != end)
  {
    if(*it == pNode)
    {
      (*it)->SetParent(nullptr);
      m_children.erase(it);      
      break;
    }
    ++it;
  }

  assert(it != end && "Attempted to detatch a node not found among children!");
}

void SceneNode::Update(double elapsedSeconds)
{
  //if(m_pParent)
  //{
  //  m_world = m_pParent->GetTransform() * m_local;
  //}
  //else
  //{
  //  
  //}

  UpdateChildren(elapsedSeconds);
}


void SceneNode::PreRender(SceneManager *pScene)
{
  pScene->PushMatrix(scaleMat * rotation * translation);
  //pScene->GetTransform();
}

void SceneNode::Render(SceneManager *pScene)
{
  RenderChildren(pScene);
}

void SceneNode::PostRender(SceneManager *pScene)
{
  pScene->PopMatrix();
}


void SceneNode::UpdateChildren(double elapsedSeconds)
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->Update(elapsedSeconds);
  }
}

void SceneNode::RenderChildren(SceneManager *pScene)
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->PreRender(pScene);
    if(m_children[i]->IsVisible())
    {
      m_children[i]->Render(pScene);
    }
    m_children[i]->PostRender(pScene);
  }
}

void SceneNode::OnDeviceLost()
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->OnDeviceLost();
  }
}

void SceneNode::OnDeviceReset()
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->OnDeviceReset();
  }
}

} // namespace shinybear