#include "scene\SceneNode.h"
#include <cassert>

namespace shinybear
{

SceneNode::SceneNode(SceneNode *pParent)
{
  m_pParent = pParent;
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

void SceneNode::Attatch(SceneNode *pNode)
{
  m_children.push_back(pNode);
}

void SceneNode::Detatch(SceneNode *pNode)
{
  auto it = m_children.begin();
  auto end = m_children.end();

  while(it != end)
  {
    if(*it == pNode)
    {
      m_children.erase(it);
      break;
    }
    ++it;
  }

  assert(it != end && "Attempted to detatch a node not found among children!");
}

void SceneNode::Update(double elapsedSeconds)
{
  UpdateChildren(elapsedSeconds);
}

void SceneNode::Render()
{
  RenderChildren();
}

void SceneNode::UpdateChildren(double elapsedSeconds)
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    m_children[i]->Update(elapsedSeconds);
  }
}

void SceneNode::RenderChildren()
{
  for(int i = 0; i < m_children.size(); ++i)
  {
    if(m_children[i]->IsVisible())
    {
      m_children[i]->Render();
    }
  }
}



} // namespace shinybear