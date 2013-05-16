#ifndef SHINYBEAR_SHADER_H
#define SHINYBEAR_SHADER_H

#include "util\math\Math.h"
#include "util\SBUtil.h"
#include "resource\types\IGraphicsResource.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <cassert>

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class Shader : public IGraphicsResource
{
public:
  Shader(GraphicsProvider *pProvider);
  ~Shader();

  void LoadFromFile(const char *pFileName);

  void SetMatrix(const char *pName, const Mat4x4 &mat);
  void SetVector4(const char *pName, const Vector4 &vec);
  void SetVector3(const char *pName, const Vector3 &vec);
  void SetVector2(const char *pName, const Vector2 &vec);
  void SetFloat(const char *pName, float val);
  void SetTexture(const char *pName, const IDirect3DTexture9 *pText);
  void SetRaw(const char *pName, void *pData, size_t size);


  void SetActiveTechnique(const char *pTechName);

  // Returns number of passes required by the technique.
  UINT Begin();

  // Stop using current effect.
  void End();

  void BeginPass(UINT index);
  void EndPass();

  void OnDeviceLost();
  void OnDeviceReset(GraphicsProvider *pProvider);

protected:

private:
  GraphicsProvider *m_pProvider;
  ID3DXEffect *m_pEffect;
};

inline void Shader::SetMatrix(const char *pName, const Mat4x4 &mat)
{
  D3DXHANDLE hMat = m_pEffect->GetParameterByName(0, pName);
  assert(hMat && "Failed to find matrix variable");
  HR(m_pEffect->SetMatrix(hMat, &mat));
}

inline void Shader::SetVector4(const char *pName, const Vector4 &vec)
{
  D3DXHANDLE hVec = m_pEffect->GetParameterByName(0, pName);
  assert(hVec && "Failed to find Vector4 variable");
  HR(m_pEffect->SetVector(hVec, &vec));
}

inline void Shader::SetVector3(const char *pName, const Vector3 &vec)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find vector3 variable");
  HR(m_pEffect->SetRawValue(hVar, (void*)&vec, 0, sizeof(Vector3)));
}

inline void Shader::SetVector2(const char *pName, const Vector2 &vec)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find vector2 variable");
  HR(m_pEffect->SetRawValue(hVar, (void*)&vec, 0, sizeof(Vector2)));
}

inline void Shader::SetFloat(const char *pName, float val)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find float variable!");
  HR(m_pEffect->SetFloat(pName, val));
}

inline void Shader::SetTexture(const char *pName, const IDirect3DTexture9 *pText)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find texture variable!");
  HR(m_pEffect->SetTexture(hVar, const_cast<IDirect3DTexture9*>(pText)));
}

inline void Shader::SetActiveTechnique(const char *pTechName)
{
  D3DXHANDLE hTech = m_pEffect->GetTechniqueByName(pTechName);
  assert(hTech && "Technique not found!");
  HR(m_pEffect->SetTechnique(hTech));
}

inline void Shader::SetRaw(const char *pName, void *pData, size_t size)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Variable not found!");
  m_pEffect->SetRawValue(hVar, pData, 0, size);
}

inline UINT Shader::Begin()
{
  UINT passes;
  HR(m_pEffect->Begin(&passes, 0));
  return passes;
}

inline void Shader::End()
{
  HR(m_pEffect->End());
}

inline void Shader::BeginPass(UINT index)
{
  HR(m_pEffect->BeginPass(index));
}

inline void Shader::EndPass()
{
  HR(m_pEffect->EndPass());
}


} // namespace shinybear

#endif