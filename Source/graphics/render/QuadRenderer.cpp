#include "graphics\render\QuadRenderer.h"
#include "graphics\VertexManager.h"
#include "base\system\GraphicsProvider.h"


namespace shinybear 
{

const DWORD QuadRenderer::kQuadFVF = D3DFVF_XYZW;

QuadRenderer::QuadRenderer(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
}

QuadRenderer::~QuadRenderer()
{

}

void QuadRenderer::Initialize()
{
  // Retrieve full screen render target declaration.
  m_pDecl = VertexManager::GetVertexDeclaration(VertexDecl::VD_DF);
}

struct V
{
  Vector4 p;
  Vector2 t;
};

void QuadRenderer::Render(const Vector2 &min, const Vector2 &max)
{
  Vector4 verts[] = 
  {
    Vector4(max.x, min.y, 0, 1),
    Vector4(min.x, min.y, 0, 1),
    Vector4(max.x, max.y, 0, 1),
    Vector4(min.x, max.y, 0, 1),
  };


  m_pDecl->Activate(); 
  IDirect3DDevice9 *pDevice = m_pProvider->GetDevice(); 
  pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, verts, sizeof(Vector4));
}

} // namespace shinybear