#include "scene\LightNode.h"
#include "scene\SceneManager.h"
#include "scene\SceneView.h"
#include "graphics\Light.h"

#include <cassert>


namespace shinybear
{

LightNode::LightNode(Light *pLight)
{
  m_pLight = pLight;
}

LightNode::~LightNode()
{
  //delete m_pLight;
}

void LightNode::OnDeviceLost()
{
  SceneNode::OnDeviceLost();
}

void LightNode::OnDeviceReset()
{
  SceneNode::OnDeviceReset();
}


void LightNode::Update(double elapsedSeconds)
{
  UpdateChildren(elapsedSeconds);
}

void LightNode::Render(SceneManager *pScene)
{
  if(m_pLight->type == LightType::LT_SPOT || m_pLight->type == LightType::LT_POINT)
  {
    m_pLight->position = GetTransform().GetPosition();
  } 
  else if(m_pLight->type == LightType::LT_DIRECTIONAL)
  {
    Vector3 pos = Mat4x4(*pScene->GetTransform()).GetPosition();
    m_pLight->direction = -pos;
  }

  RenderData rd;
  rd.data.pLight = m_pLight;
  rd.type = RenderType::LIGHT;
  rd.world = *(pScene->GetTransform());
  pScene->PushRenderData(rd);

  RenderChildren(pScene);
}

} // namespace shinybear