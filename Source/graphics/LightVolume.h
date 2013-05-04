#ifndef SHINYBEAR_LIGHTVOLUME_H
#define SHINYBEAR_LIGHTVOLUME_H

#include "util\math\Math.h"
#include <d3dx9.h>

struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

namespace shinybear
{

class LightVolume
{
public:
  static void Initialize();

  void OnDeviceReset();
  void OnDeviceLost();

  // Returns a lightvolume as a unit quad.
  static const LightVolume *GetRect();

  // returns a unit sphere lightvolume
  static const LightVolume *GetSphere();

  // returns a cone with 1 in length and 
  static const LightVolume *GetCone();

private:
  struct VertexData
  {
    Vector3 position;
  };

  LightVolume();
  ~LightVolume();

  static void CreateRect();
  static void CreateSphere();
  static void CreateCone();

  static LightVolume *pRect;
  static LightVolume *pSphere;
  static LightVolume *pCone;

  IDirect3DVertexBuffer9 *m_pVBuffer;
  IDirect3DIndexBuffer9 *m_pIBuffer;

};

}

#endif