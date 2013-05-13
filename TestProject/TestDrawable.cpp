#include "TestDrawable.h"
#include <base\system\GraphicsProvider.h>
#include <graphics\Mesh.h>

using namespace shinybear;

TestDrawable::TestDrawable()
{
  m_world = Mat4x4::kIdentity;
  m_world = Mat4x4::CreateScale(5.0f);
}

TestDrawable::~TestDrawable()
{
  delete m_pMesh;
}

void TestDrawable::Render(GraphicsProvider *pProvider)
{
  IDirect3DDevice9 *pDevice = pProvider->GetDevice();
  
  m_pMesh->RenderMesh();
}

