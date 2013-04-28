#ifndef SHINYBEAR_SHADER_H
#define SHINYBEAR_SHADER_H

#include "graphics\shader\IShader.h"
#include "util\SBUtil.h"
#include <d3dx9.h>
#include <cassert>

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class Shader : public IShader
{
public:
  Shader(GraphicsProvider *pProvider);
  
  ~Shader();
  
  virtual void LoadFromFile(const wchar_t *pFileName);

  virtual void SetActiveTechnique(UINT techIndex);
  virtual void SetActiveTechnique(const char *pTechName);
  virtual void SetActivePass(UINT passIndex);
  virtual void SetActivePass(const char *pPassName);

  virtual void Begin();
  virtual void End();


  virtual void DrawIndexedPrimitives(D3DPRIMITIVETYPE type,
    int baseVertex, UINT minIndex, UINT count, UINT startIndex,
    UINT primCount);
protected:
  ID3DXEffect *m_pEffect;
  GraphicsProvider *m_pProvider;

private:
  size_t m_passCount;
  size_t m_techCount;
  D3DXHANDLE m_currentTechnique;
  D3DXHANDLE m_currentPass;
  UINT m_passesNeeded;
  bool m_beginCalled;
};

inline void Shader::Begin()
{
  HR(m_pEffect->Begin(&m_passesNeeded, 0));
  m_beginCalled = true;
}

inline void Shader::End()
{
  assert(m_beginCalled && "Begin must be called before end!");
  HR(m_pEffect->End());
  m_beginCalled = false;
}

} // namespace shinybear

#endif // SHINYBEAR_SHADER_H