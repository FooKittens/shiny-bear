#include "scene\LightNode.h"
#include "scene\SceneManager.h"
#include "scene\SceneView.h"
#include "graphics\Light.h"

#include <cassert>


namespace shinybear
{

LightNode::LightNode(Light light)
{
  m_light = light;
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
  if(m_light.type == LightType::LT_SPOT || m_light.type == LightType::LT_POINT)
  {
    m_light.position = GetTransform().GetPosition();
  } 
  else if(m_light.type == LightType::LT_DIRECTIONAL)
  {
    Vector3 pos = Mat4x4(*pScene->GetTransform()).GetPosition();
    m_light.direction = -pos;
  }

  RenderData rd;
  rd.data.pLight = &m_light;
  rd.type = RenderType::LIGHT;
  rd.world = *(pScene->GetTransform());
  pScene->PushRenderData(rd);

  RenderChildren(pScene);
}

} // namespace shinybear