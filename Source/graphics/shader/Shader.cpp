#include "graphics\shader\Shader.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"
#include <cassert>

namespace shinybear
{

Shader::Shader(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
  m_pEffect = nullptr;
  m_beginCalled = false;
}

Shader::~Shader()
{
  RELEASECOM(m_pEffect);
}

void Shader::LoadFromFile(const wchar_t *pFileName)
{
  DWORD flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
  flags |= D3DXSHADER_DEBUG;
#endif
  ID3DXBuffer *pBuffer;

  D3DXCreateEffectFromFileW(
    m_pProvider->GetDevice(),
    pFileName,
    NULL,
    NULL,
    flags,
    NULL,
    &m_pEffect,
    &pBuffer
  );

  if(pBuffer)
  {
    OutputDbgFormat("SHADER ERROR: %s\n",(char*)pBuffer->GetBufferPointer());
    assert(false && "Shader error encountered, see output window!");
  }
}

void Shader::SetActiveTechnique(UINT techIndex)
{
  assert(techIndex < m_techCount && "TechniqueIndex out of bounds!");
  m_currentTechnique = m_pEffect->GetTechnique(techIndex);
  m_pEffect->SetTechnique(m_currentTechnique);
}

void Shader::SetActiveTechnique(const char *pTechName)
{
  m_currentTechnique = m_pEffect->GetTechniqueByName(pTechName);
  assert(m_currentTechnique && "Technique not found!");
  m_pEffect->SetTechnique(m_currentTechnique);
}

void Shader::SetActivePass(UINT passIndex)
{
  m_currentPass = m_pEffect->GetPass(m_currentTechnique, passIndex);
  assert(m_currentPass && "Failed to find pass!");
}

void Shader::SetActivePass(const char *pPassName)
{
  m_currentPass = m_pEffect->GetPassByName(m_currentTechnique, pPassName);
  assert(m_currentPass && "Failed to find pass!");
}

void Shader::DrawIndexedPrimitives(D3DPRIMITIVETYPE type, int baseVertex,
  UINT minIndex, UINT count, UINT startIndex, UINT primCount)
{
  assert(m_beginCalled && "Shader::Begin() must be called before rendering!");
  
  m_pProvider->GetDevice()->DrawIndexedPrimitive(type, baseVertex,
    minIndex, count, startIndex, primCount);
}

} // namespace shinybear