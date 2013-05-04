#ifndef SHINYBEAR_SHADER_H
#define SHINYBEAR_SHADER_H

#include "util\math\Math.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <cassert>

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class Shader
{
public:
  Shader(GraphicsProvider *pProvider);
  ~Shader();

  void LoadFromFile(const wchar_t *pFileName);

  void SetMatrix(const char *pName, const Mat4x4 &mat);
  void SetVector4(const char *pName, const Vector4 &vec);
  void SetVector3(const char *pName, const Vector3 &vec);
  void SetVector2(const char *pName, const Vector2 &vec);
  void SetFloat(const char *pName, float val);
  void SetTexture(const char *pName, const IDirect3DTexture9 *pText);
protected:

private:
  GraphicsProvider *m_pProvider;
  ID3DXEffect *m_pEffect;
};

inline void Shader::SetMatrix(const char *pName, const Mat4x4 &mat)
{
  D3DXHANDLE hMat = m_pEffect->GetParameterByName(0, pName);
  assert(hMat && "Failed to find matrix variable");
  m_pEffect->SetMatrix(hMat, &mat);
}

inline void Shader::SetVector4(const char *pName, const Vector4 &vec)
{
  D3DXHANDLE hVec = m_pEffect->GetParameterByName(0, pName);
  assert(hVec && "Failed to find Vector4 variable");
  m_pEffect->SetVector(hVec, &vec);
}

inline void Shader::SetVector3(const char *pName, const Vector3 &vec)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find vector3 variable");
  m_pEffect->SetRawValue(hVar, (void*)&vec, 0, sizeof(Vector3));
}

inline void Shader::SetVector2(const char *pName, const Vector2 &vec)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find vector2 variable");
  m_pEffect->SetRawValue(hVar, (void*)&vec, 0, sizeof(Vector2));
}

inline void Shader::SetFloat(const char *pName, float val)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find float variable!");
  m_pEffect->SetFloat(pName, val);
}

inline void Shader::SetTexture(const char *pName, const IDirect3DTexture9 *pText)
{
  D3DXHANDLE hVar = m_pEffect->GetParameterByName(0, pName);
  assert(hVar && "Failed to find texture variable!");
  m_pEffect->SetTexture(hVar, const_cast<IDirect3DTexture9*>(pText));
}

} // namespace shinybear

#endif