#ifndef SHINYBEAR_CONFIG_H
#define SHINYBEAR_CONFIG_H

#include "base\system\DisplayMode.h"
#include <fstream>

namespace shinybear
{

struct Config
{
  Config() {}
  Config(std::wfstream &in);

  DisplayMode displayMode;
  MultiSampleMode multiSampleMode;

  // Save to stream
  void ToText(std::wfstream &out) const;
  void FromText(std::wfstream &in);

  static Config GetDefault();
};

} // namespace shinybear

#endif // SHINYBEAR_CONFIG_H