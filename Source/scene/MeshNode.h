#ifndef SHINYBEAR_MESHNODE_H
#define SHINYBEAR_MESHNODE_H

#include "scene\SceneNode.h"

namespace shinybear
{

class MeshNode : public SceneNode
{
public:
  MeshNode(SceneNode *pParent);
  ~MeshNode();
protected:

private:
  
};

} // namespace shinybear

#endif // SHINYBEAR_MESHNODE_H
