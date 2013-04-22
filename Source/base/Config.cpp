#include "base\Config.h"
#include <string>
#include <cassert>

using std::wfstream;
using std::endl;
using std::wstring;

namespace shinybear
{


Config::Config(wfstream &in)
{
  this->FromText(in);
}

void Config::ToText(wfstream &out) const
{
  out << L"--DisplayMode--" << endl
      << L"width = " << displayMode.width << endl
      << L"height = " << displayMode.height << endl
      << L"refresh = " << displayMode.refreshRate << endl
      << L"fullscreen = " << (int)displayMode.fullscreen << endl
        
      << L"--MSAA--" << endl
      << L"samples = " << multiSampleMode.samples << endl
      << L"quality = " << multiSampleMode.quality << endl;

  out.flush();
}

void Config::FromText(wfstream &in)
{
  
  wstring input;

  in >> input;
  assert(input.compare(L"--DisplayMode--") == 0 && "Invalid Stream data!");

  // Remove name and '=' before each value.
  in >> input >> input >> displayMode.width;    
  in >> input >> input >> displayMode.height;
  in >> input >> input >> displayMode.refreshRate;
  in >> input >> input >> displayMode.fullscreen;
    
  in >> input;
  assert(input.compare(L"--MSAA--") == 0 && "Invalid Stream data!");
  in >> input >> input >> multiSampleMode.samples;
  in >> input >> input >> multiSampleMode.quality;
}

Config Config::GetDefault()
{
  Config config;
  config.displayMode.fullscreen = false;
  config.displayMode.height = 480;
  config.displayMode.width = 640;
  config.displayMode.refreshRate = 60;
  config.multiSampleMode.quality = 0;
  config.multiSampleMode.samples = 1;

  return config;
}

} // namespace shinybear