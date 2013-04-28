#ifndef SHINYBEAR_SIMPLESHADER_H
#define SHINYBEAR_SIMPLESHADER_H

#include "graphics\shader\Shader.h"

namespace shinybear
{

class LightShader : public Shader
{
public:
  LightShader(GraphicsProvider *pProvider, const wchar_t *pPath);
  ~LightShader();

  void SetWorldMatrix(const D3DXMATRIX &mat);
  void SetViewMatrix(const D3DXMATRIX &mat);
  void SetProjectionMatrix(const D3DXMATRIX &mat);

protected:

private:
  D3DXHANDLE m_hWorld, m_hView, m_hProjection;
};

inline void LightShader::SetWorldMatrix(const D3DXMATRIX &mat)
{
  m_pEffect->SetMatrix(m_hWorld, &mat);
}

inline void LightShader::SetViewMatrix(const D3DXMATRIX &mat)
{
  m_pEffect->SetMatrix(m_hView, &mat);
}

inline void LightShader::SetProjectionMatrix(const D3DXMATRIX &mat)
{
  m_pEffect->SetMatrix(m_hProjection, &mat);
}

} // namespace shinybear

#endif // SHINYBEAR_SIMPLESHADER_H