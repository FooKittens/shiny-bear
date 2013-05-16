#ifndef SHINYBEAR_LIGHTING_H
#define SHINYBEAR_LIGHTING_H

#include "util\math\Math.h"

#include <d3dx9.h>

namespace shinybear
{

namespace LightType
{
  enum E
  {
    // Invalid light type used to find bugs.
    LT_UNDEFINED = 0,

    LT_AMBIENT = 1,
    LT_DIRECTIONAL,
    LT_POINT,
    LT_SPOT
  };
}

// Abstract class that other lights may inherit form.
class BaseLight
{
public:
  BaseLight(const D3DXCOLOR &color) : m_color(color) { }

  // Default constructor with white light only.
  BaseLight() : m_color(D3DXCOLOR(1, 1, 1, 1)) { }

  // Virtual destructor to ensure correct
  // destroying of sub classes.
  virtual ~BaseLight() { }

  // All lights come with a color.
  virtual const D3DXCOLOR &GetColor() const { return m_color; }
  virtual void SetColor(const D3DXCOLOR &color) { m_color = color; }

  // Used to determine the type of light in rendering.
  virtual LightType::E GetType() const { return LightType::LT_AMBIENT; }

protected:
  D3DXCOLOR m_color;
};

class DirectionalLight : public BaseLight
{
public:
  // constructor for specifying both a color and a direction.
  DirectionalLight(const D3DXCOLOR &color, const Vector3 &direction)
    :BaseLight(color), m_direction(direction) { }

  // Default constructor with white light.
  DirectionalLight(const Vector3 &direction) : m_direction(direction) { }

  LightType::E GetType() const { return LightType::LT_DIRECTIONAL; }

  // Allow for simple editing.
  Vector3 m_direction; 
};


class PointLight : public BaseLight
{
public:
  PointLight(const D3DXCOLOR &color, const Vector3 &position,
    float dimFactor, float range)
    :BaseLight(color), m_position(position), m_dim(dimFactor), m_range(range)
  {
    SetAttuneation();
  }

  PointLight(const Vector3 &position, float dimFactor = 0.5f, float range = 25.0f)
    :m_position(position), m_dim(dimFactor), m_range(range)
  {
    SetAttuneation();
  }

  LightType::E GetType() const { return LightType::LT_POINT; }

  // Helps calculate attuneation factors from range and dimfactor.
  void SetAttuneation()
  {
    att0 = m_dim;
    att1 = 4.5f / m_range;
    att2 = 75.0f / (m_range * m_range);
  }
  
  // For easy editing.
  Vector3 m_position;
  float m_dim;
  float m_range;
  float att0;
  float att1;
  float att2;
};

} // namespace shinybear

#endif // SHINYBEAR_ILIGHT_H