#include "world\Cluster.h"
#include "world\Block.h"
#include "scene\SceneManager.h"
#include "scene\MeshNode.h"
#include "graphics\Mesh.h"
#include "util\SBUtil.h"

namespace shinybear
{

Cluster::Cluster()
{
  m_recreateMesh = true;
  m_pMeshNode = nullptr;
  m_blocks = DBG_NEW Block**[kSizeX];
  for(int x = 0; x < kSizeX; ++x)
  {
    m_blocks[x] = DBG_NEW Block*[kSizeY];
    for(int y = 0; y < kSizeY; ++y)
    {
      m_blocks[x][y] = DBG_NEW Block[kSizeZ];
    }
  }
}

Cluster::~Cluster()
{
  for(int x = 0; x < kSizeX; ++x)
  {
    for(int y = 0; y < kSizeY; ++y)
    {
      delete[] m_blocks[x][y];
    }
    delete[] m_blocks[x];
  }
  delete[] m_blocks;
}

void Cluster::Render(SceneManager *pScene)
{
  if(m_recreateMesh)
  {
    RecreateMesh();
    m_recreateMesh = false;
  }

  RenderChildren(pScene);
}

void Cluster::RecreateMesh()
{
  Mesh *pMesh = m_pMeshNode->GetMesh();
  for(int x = 0; x < kSizeX; ++x)
  {
    for(int y = 0; y < kSizeY; ++y)
    {
      for(int z = 0; z < kSizeZ; ++z)
      {
        if(!m_blocks[x][y][z].IsVisible())
        {
          continue;
        }
        
        float blockX = (float)x - (float)kSizeX / 2.0f;
        float blockY = (float)y - (float)kSizeY / 2.0f;
        float blockZ = (float)z - (float)kSizeZ / 2.0f;

        int hideFlags = 0;
        if(x > 0 && m_blocks[x - 1][y][z].IsVisible())
          hideFlags |= HF_LEFT;

        if(x < kSizeX && m_blocks[x + 1][y][z].IsVisible())
          hideFlags |= HF_RIGHT;

        if(y > 0 && m_blocks[x][y - 1][z].IsVisible())
          hideFlags |= HF_BOTTOM;

        if(y < kSizeY && m_blocks[x][y + 1][z].IsVisible())
          hideFlags |= HF_TOP;

        if(z > 0 && m_blocks[x][y][z - 1].IsVisible())
          hideFlags |= HF_FRONT;

        if(z < kSizeZ && m_blocks[x][y][z + 1].IsVisible())
          hideFlags |= HF_BACK;

        CreateCube(blockX, blockY, blockZ, m_blocks[x][y][z], pMesh, hideFlags);
      }
    }
  }

  pMesh->UpdateBuffers();
}

void Cluster::CreateCube(float x, float y, float z, const Block &block,
  Mesh *pMesh, int hideFlags)
{
  static const D3DXVECTOR3 kFrontNormal   = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
  static const D3DXVECTOR3 kBackNormal    = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
  static const D3DXVECTOR3 kTopNormal     = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
  static const D3DXVECTOR3 kBottomNormal  = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
  static const D3DXVECTOR3 kLeftNormal    = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
  static const D3DXVECTOR3 kRightNormal   = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

  // Front positions.
  const D3DXVECTOR3 p0 = D3DXVECTOR3(x - 0.5f, y + 0.5f, z - 0.5f);
  const D3DXVECTOR3 p1 = D3DXVECTOR3(x + 0.5f, y + 0.5f, z - 0.5f);
  const D3DXVECTOR3 p2 = D3DXVECTOR3(x + 0.5f, y - 0.5f, z - 0.5f);
  const D3DXVECTOR3 p3 = D3DXVECTOR3(x - 0.5f, y - 0.5f, z - 0.5f);

  // Back Positions
  const D3DXVECTOR3 p4 = D3DXVECTOR3(x + 0.5f, y + 0.5f, z + 0.5f);
  const D3DXVECTOR3 p5 = D3DXVECTOR3(x - 0.5f, y + 0.5f, z + 0.5f);
  const D3DXVECTOR3 p6 = D3DXVECTOR3(x - 0.5f, y - 0.5f, z + 0.5f);
  const D3DXVECTOR3 p7 = D3DXVECTOR3(x + 0.5f, y - 0.5f, z + 0.5f);

  UINT v0, v1, v2, v3;
  BlockMaterial mat = block.GetMaterial();
  if(!(hideFlags & HF_FRONT))
  {  
    v0 = pMesh->AddVertex(p0, mat, kFrontNormal);
    v1 = pMesh->AddVertex(p1, mat, kFrontNormal);
    v2 = pMesh->AddVertex(p2, mat, kFrontNormal);
    v3 = pMesh->AddVertex(p3, mat, kFrontNormal);
    
    pMesh->AddTriangle(v0, v1, v2);
    pMesh->AddTriangle(v2, v3, v0);
  }

  if(!(hideFlags & HF_BACK))
  {
    v0 = pMesh->AddVertex(p4, mat, kBackNormal);
    v1 = pMesh->AddVertex(p5, mat, kBackNormal);
    v2 = pMesh->AddVertex(p6, mat, kBackNormal);
    v3 = pMesh->AddVertex(p7, mat, kBackNormal);
    
    pMesh->AddTriangle(v0, v1, v2);
    pMesh->AddTriangle(v2, v3, v0);
  }

  if(!(hideFlags & HF_TOP))
  {
    v0 = pMesh->AddVertex(p5, mat, kTopNormal);
    v1 = pMesh->AddVertex(p4, mat, kTopNormal);
    v2 = pMesh->AddVertex(p1, mat, kTopNormal);
    v3 = pMesh->AddVertex(p0, mat, kTopNormal);
    
    pMesh->AddTriangle(v0, v1, v2);
    pMesh->AddTriangle(v2, v3, v0);
  }

  if(!(hideFlags & HF_BOTTOM))
  {
    v0 = pMesh->AddVertex(p3, mat, kBottomNormal);
    v1 = pMesh->AddVertex(p2, mat, kBottomNormal);
    v2 = pMesh->AddVertex(p7, mat, kBottomNormal);
    v3 = pMesh->AddVertex(p6, mat, kBottomNormal);
    
    pMesh->AddTriangle(v0, v1, v2);
    pMesh->AddTriangle(v2, v3, v0);
  }

  if(!(hideFlags & HF_LEFT))
  {
    v0 = pMesh->AddVertex(p5, mat, kLeftNormal);
    v1 = pMesh->AddVertex(p0, mat, kLeftNormal);
    v2 = pMesh->AddVertex(p3, mat, kLeftNormal);
    v3 = pMesh->AddVertex(p6, mat, kLeftNormal);
    
    pMesh->AddTriangle(v0, v1, v2);
    pMesh->AddTriangle(v2, v3, v0);
  }

  if(!(hideFlags & HF_RIGHT))
  {
    v0 = pMesh->AddVertex(p1, mat, kRightNormal);
    v1 = pMesh->AddVertex(p4, mat, kRightNormal);
    v2 = pMesh->AddVertex(p7, mat, kRightNormal);
    v3 = pMesh->AddVertex(p2, mat, kRightNormal);
    
    pMesh->AddTriangle(v0, v1, v2);
    pMesh->AddTriangle(v2, v3, v0);
  }
}

} // namespace shinybear