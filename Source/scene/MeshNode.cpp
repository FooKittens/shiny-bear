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


void MeshNode::Render(SceneManager *pScene)
{
  RenderChildren(pScene);

  RenderData rd;
  rd.data.pMesh = m_pMesh;
  rd.type = RenderType::MESH;
  rd.world = *(pScene->GetTransform());
  pScene->PushRenderData(rd);

  
}

void MeshNode::OnDeviceLost()
{
  SceneNode::OnDeviceLost();
}

void MeshNode::OnDeviceReset()
{
  SceneNode::OnDeviceReset();
  m_pMesh->UpdateBuffers();
}

} // namespace shinybear