#ifndef SHINYBEAR_ISHADER_H
#define SHINYBEAR_ISHADER_H

#include <d3dx9.h>

namespace shinybear
{

class IShader
{
public:
  IShader() { }
  virtual ~IShader() { }

  virtual void SetWorldMatrix(const D3DXMATRIX &mat) = 0;
  virtual void SetViewMatrix(const D3DXMATRIX &mat) = 0;
  virtual void SetProjectionMatrix(const D3DXMATRIX &mat) = 0;
protected:

private:
};



} // namespace shinybear

#endif // SHINYBEAR_ISHADER_H