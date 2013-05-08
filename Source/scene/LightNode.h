#ifndef SHINYBEAR_LIGHTNODE_H
#define SHINYBEAR_LIGHTNODE_H

#include "scene\SceneNode.h"
#include "graphics\Light.h"

namespace shinybear { struct Light; }

namespace shinybear
{

class LightNode : public SceneNode
{
public:
  LightNode(Light Light);
  ~LightNode();

  void Update(double);
  void Render(SceneManager *pScene);

  void OnDeviceLost();
  void OnDeviceReset();

protected:

private:
  Light m_light;
};

} // namespace shinybear

#endif // SHINYBEAR_LIGHTNODE_H
