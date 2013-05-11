#include "graphics\render\QuadRenderer.h"
#include "base\system\GraphicsProvider.h"

namespace shinybear 
{

QuadRenderer::QuadRenderer(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
}

void QuadRenderer::Initialize()
{

}

} // namespace shinybear