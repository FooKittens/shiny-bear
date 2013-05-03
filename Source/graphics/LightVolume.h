#ifndef SHINYBEAR_LIGHTVOLUME_H
#define SHINYBEAR_LIGHTVOLUME_H

#include <vector>

namespace shinybear
{

class LightVolume
{
public:
  LightVolume();
  ~LightVolume();

  void OnDeviceReset();
  void OnDeviceLost();

private:

};

}

#endif