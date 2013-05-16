#include "graphics\Shader.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"



namespace shinybear
{

Shader::Shader(GraphicsProvider *pProvider)
  :m_pProvider(pProvider)
{
  m_pEffect = nullptr;
}

Shader::~Shader()
{
  RELEASECOM(m_pEffect);
}

void Shader::LoadFromFile(const char *pFileName)
{
  assert(!m_pEffect && "Shader already loaded!");

  DWORD flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  flags |= D3DXSHADER_DEBUG;
#endif

  ID3DXBuffer *pCompErrors;

  HR(D3DXCreateEffectFromFile(
    m_pProvider->GetDevice(),
    pFileName,
    0,
    0,
    flags,
    0,
    &m_pEffect,
    &pCompErrors
  ));

  if(pCompErrors)
  {
    OutputDbgFormat("SHADER COMPILATION ERRORS: %s",
      (char *)(pCompErrors->GetBufferPointer()));
  }
}

void Shader::OnDeviceLost()
{
  HR(m_pEffect->OnLostDevice());
}

void Shader::OnDeviceReset(GraphicsProvider *pProvider)
{
  HR(m_pEffect->OnResetDevice());
}

} // namespace shader