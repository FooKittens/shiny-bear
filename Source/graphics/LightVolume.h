#ifndef SHINYBEAR_LIGHTVOLUME_H
#define SHINYBEAR_LIGHTVOLUME_H

#include "util\math\Math.h"
#include <d3dx9.h>

struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

class LightVolume
{
public:
  LightVolume(GraphicsProvider *pProvider);
  ~LightVolume();

  void OnDeviceReset();
  void OnDeviceLost();

  // Used for creating different shapes.
  void MakeQuad();
  void MakeSphere();
  void MakeCone();

  void Render();

private:
  struct VertexData
  {
    Vector3 position;
  };
  enum Type
  {
    LV_QUAD = 1,
    LV_SPHERE = 2,
    LV_CONE = 3,
  } m_type;


  UINT m_vertexCount;
  UINT m_indexCount;

  IDirect3DVertexBuffer9 *m_pVBuffer;
  IDirect3DIndexBuffer9 *m_pIBuffer;

  GraphicsProvider *m_pProvider;
};

}

#endif