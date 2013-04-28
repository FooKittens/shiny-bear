#include "graphics\shader\LightShader.h"
#include "base\system\GraphicsProvider.h"
#include <cassert>

namespace shinybear
{

LightShader::LightShader(GraphicsProvider *pProvider, const wchar_t *pPath)
  :Shader(pProvider)
{
  DWORD flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  flags |= D3DXSHADER_DEBUG;
#endif

  m_hWorld = m_pEffect->GetParameterByName(NULL, "g_world");
  m_hView = m_pEffect->GetParameterByName(NULL, "g_view");
  m_hProjection = m_pEffect->GetParameterByName(NULL, "g_projection");
}

LightShader::~LightShader()
{

}

} // namespace shinybear