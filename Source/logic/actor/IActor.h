#ifndef SHINYBEAR_IACTOR_H
#define SHINYBEAR_IACTOR_H


struct D3DXMATRIX;
namespace shinybear { class Mesh; }

namespace shinybear
{

class IActor
{
public:
  virtual const D3DXMATRIX *GetWorldMatrix() const = 0;
  virtual const Mesh *GetMesh() const = 0;
};

} // namespace shinybear

#endif // SHINYBEAR_IACTOR_H