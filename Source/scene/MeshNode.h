#ifndef SHINYBEAR_MESHNODE_H
#define SHINYBEAR_MESHNODE_H

#include "scene\SceneNode.h"

namespace shinybear { class Mesh; }

namespace shinybear
{

class MeshNode : public SceneNode
{
public:
  MeshNode(Mesh *pMesh);
  ~MeshNode();

  void Render(SceneManager *pScene);

  void OnDeviceLost();
  void OnDeviceReset();

  Mesh *GetMesh() const;
  
protected:

private:
  Mesh *m_pMesh;
};

inline Mesh *MeshNode::GetMesh() const
{
  return m_pMesh;
}

} // namespace shinybear

#endif // SHINYBEAR_MESHNODE_H
