#include "graphics\LightVolume.h"

#include <cassert>


namespace shinybear
{

LightVolume::LightVolume()
{

}

LightVolume::~LightVolume()
{

}

void LightVolume::OnDeviceReset()
{

}

void LightVolume::OnDeviceLost()
{

}

void LightVolume::CreateRect()
{
  assert(!pRect && "Rectangle volume already exists!");

  VertexData vertices[] = 
  {
    { Vector3(-0.5f, +0.5f, 0) },
    { Vector3(+0.5f, +0.5f, 0) },
    { Vector3(+0.5f, -0.5f, 0) },
    { Vector3(-0.5f, -0.5f, 0) },
  };


}


} // namespace shinybear