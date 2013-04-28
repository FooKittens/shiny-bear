#ifndef SHINYBEAR_LIGHTNODE_H
#define SHINYBEAR_LIGHTNODE_H

#include "scene\SceneNode.h"

namespace shinybear { struct Light; }

namespace shinybear
{

class LightNode : public SceneNode
{
public:
  LightNode(Light *pLight);
  ~LightNode();

  void Update(double);
  void Render(SceneView *pRenderer);

protected:

private:
  Light *m_pLight;
};

} // namespace shinybear

#endif // SHINYBEAR_LIGHTNODE_H
