#include "scene\MeshNode.h"
#include "scene\SceneManager.h"
#include "scene\SceneView.h"
#include "graphics\Mesh.h"


namespace shinybear
{

MeshNode::MeshNode(Mesh *pMesh)
{
  m_pMesh = pMesh;
}

MeshNode::~MeshNode()
{
  delete m_pMesh;
}

void MeshNode::Render(SceneView *pRenderer)
{
  pRenderer->AddMeshNode(this);
  RenderChildren(pRenderer);
}

void MeshNode::OnDeviceLost()
{

}

void MeshNode::OnDeviceReset()
{
  m_pMesh->UpdateBuffers();
}

} // namespace shinybear