#ifndef SHINYBEAR_LIGHT_H
#define SHINYBEAR_LIGHT_H

#include "util\math\Math.h"

namespace shinybear
{

namespace LightType
{
enum Enum
{
  LT_AMBIENT = 1, 
  LT_DIRECTIONAL = 2,
  LT_POINT = 3,
  LT_SPOT = 4,
};
}
  
struct Light
{
  LightType::Enum type;
  float filler_1;
  float filler_2;
  float filler_3;
  Vector3 position;
  float filler;
  Vector3 direction;
  float filler2;
  D3DXCOLOR color;
  float filler3;
    
  // Should only be created through factory methods.
  Light() { }

  static Light CreateAmbientLight(const D3DXCOLOR &color)
  {
    Light lt;
    lt.type = LightType::LT_AMBIENT;
    lt.color = color;
    return lt;
  }

  static Light CreateDirectionalLight(const D3DXCOLOR &color,
    const Vector3 &direction)
  {
    Light lt;
    lt.type = LightType::LT_DIRECTIONAL;
    lt.color = color;
    lt.direction = direction;
    return lt;
  }

private:

};






} // namespace shinybear
#endif // SHINYBEAR_LIGHT_H