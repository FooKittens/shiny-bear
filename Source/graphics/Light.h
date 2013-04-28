#ifndef SHINYBEAR_LIGHT_H
#define SHINYBEAR_LIGHT_H

#include <d3dx9.h>

namespace shinybear
{

enum LightType
{
  LT_AMBIENT, 
  LT_DIRECTIONAL,
  LT_POINT,
  LT_SPOT,
};
  
struct Light
{
  LightType type;
  D3DXVECTOR3 position;
  D3DXVECTOR3 direction;
  D3DXCOLOR color;
  float influenceRadius;
};

struct AmbientLight : public Light
{
  AmbientLight(const D3DXCOLOR &color) { Setup(color); }
  AmbientLight() { Setup(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f)); }

private:
  void Setup(const D3DXCOLOR &color)
  {
    type = LT_AMBIENT;
    position = D3DXVECTOR3(0, 0, 0);
    direction = D3DXVECTOR3(0, 0, 0);
    this->color = color;
    influenceRadius = 0;
  }
};

struct DirectionalLight : public Light
{
  DirectionalLight(const D3DXCOLOR &color, const D3DXVECTOR3 &direction)
  {
    Setup(color, direction);
  }

  DirectionalLight(const D3DXVECTOR3 &dir)
  {
    Setup(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), dir);
  }

private:
  void Setup(const D3DXCOLOR &color, const D3DXVECTOR3 &direction)
  {
    type = LT_DIRECTIONAL;
    position = D3DXVECTOR3(0, 0, 0);
    this->direction = direction;
    this->color = color;
    influenceRadius = 0;
  }
};

struct PointLight : public Light
{

};

struct SpotLight : public Light
{

};




} // namespace shinybear
#endif // SHINYBEAR_LIGHT_H