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
  if(m_pParent)
  {
    m_world = m_pParent->GetTransform() * m_local;
  }
  else
  {
    m_world = m_local;
  }
  UpdateChildren(elapsedSeconds);
}


void SceneNode::Render(SceneView *pRenderer)
{
  RenderChildren(pRenderer);
}


void SceneNode::UpdateChildren(double elapsedSeconds)
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->Update(elapsedSeconds);
  }
}

void SceneNode::RenderChildren(SceneView *pRenderer)
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    if(m_children[i]->IsVisible())
    {
      m_children[i]->Render(pRenderer);
    }
  }
}



} // namespace shinybear