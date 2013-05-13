#ifndef SHINYBEAR_LIGHTVOLUME_H
#define SHINYBEAR_LIGHTVOLUME_H

#include "resource\types\IGraphicsResource.h"
#include "util\math\Math.h"
#include <d3dx9.h>
#include <vector>


struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

// Used when making sphere meshes.
struct Triangle
{
  Triangle(UINT v0, UINT v1, UINT v2)
    :v0(v0), v1(v1), v2(v2) { }
  UINT v0, v1, v2;
};

class LightVolume : public IGraphicsResource
{
public:
  LightVolume(GraphicsProvider *pProvider);
  ~LightVolume();

  void OnDeviceReset(GraphicsProvider *pProvider);
  void OnDeviceLost();

  // Used for creating different shapes.
  void MakeQuad();
  void MakeSphere();
  void MakeCone();

  void Render();

private:
  struct VertexData
  {
    Vector4 position;
  };
  enum Type
  {
    LV_QUAD = 1,
    LV_SPHERE = 2,
    LV_CONE = 3,
  } m_type;

  // used to split icohedradon edges for spheres.
  UINT SplitEdge(std::vector<Vector4> &verts, const Vector4 &v0, const Vector4 &v1);

  void CreateSphereData(UINT levels, std::vector<Vector4> &verts,
    std::vector<Triangle> &tris);

  UINT m_vertexCount;
  UINT m_indexCount;

  IDirect3DVertexBuffer9 *m_pVBuffer;
  IDirect3DIndexBuffer9 *m_pIBuffer;

  GraphicsProvider *m_pProvider;
};

}

#endif