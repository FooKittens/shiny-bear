#ifndef SHINYBEAR_LIGHT_H
#define SHINYBEAR_LIGHT_H

#include "util\math\Math.h"

#include <memory>

namespace shinybear
{

namespace LightTypeHURRR
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
  LightTypeHURRR::Enum type;
  
  float filler_2;
  float filler_3;
  float filler;
  Vector3 direction;
  float range;
  D3DXCOLOR color;
  Vector3 attenuation;
    
  // Should only be created through factory methods.
  Light() { }

  static Light CreateAmbientLight(const D3DXCOLOR &color)
  {
    Light lt;
    lt.type = LightTypeHURRR::LT_AMBIENT;
    lt.color = color;
    return lt;
  }

  static Light CreateDirectionalLight(const D3DXCOLOR &color,
    const Vector3 &direction)
  {
    Light lt;
    lt.type = LightTypeHURRR::LT_DIRECTIONAL;
    lt.color = color;
    lt.direction = direction;
    return lt;
  }

  static Light CreatePointLight(const D3DXCOLOR &color,
    const Vector3 &position, float dimFactor, float range)
  {
    Light lt;
    lt.type = LightTypeHURRR::LT_POINT;
    lt.color = color;
    lt.position = position;
    lt.attenuation = Vector3(dimFactor, 4.5f / range, 85.0f / (range * range));
    lt.range = range;
    return lt;
  }

private:

};





} // namespace shinybear
#endif // SHINYBEAR_LIGHT_H