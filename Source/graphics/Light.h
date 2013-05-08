#ifndef SHINYBEAR_LIGHT_H
#define SHINYBEAR_LIGHT_H

#include "util\math\Math.h"

namespace shinybear
{

namespace LightType
{
enum Enum : UINT
{
  LT_AMBIENT = 1, 
  LT_DIRECTIONAL = 2,
  LT_POINT = 3,
  LT_SPOT = 4,
};
}
  
struct Light
{
  Vector3 position;
  float filler_1;
  LightType::Enum type;
  
  float filler_2;
  float filler_3;
  float filler;
  Vector3 direction;
  float filler2;
  D3DXCOLOR color;
  Vector3 attenuation;
    
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

  static Light CreatePointLight(const D3DXCOLOR &color,
    const Vector3 &position, const Vector3 &attenuation)
  {
    Light lt;
    lt.type = LightType::LT_POINT;
    lt.color = color;
    lt.position = position;
    lt.attenuation = attenuation;
    return lt;
  }

private:

};






} // namespace shinybear
#endif // SHINYBEAR_LIGHT_H