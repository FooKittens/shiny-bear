#include "sound\WaveFile.h"
#include "util\SBUtil.h"

#include <dsound.h>
#include <cassert>
#include <stdio.h>
#include <iostream>
#include <fstream>

namespace shinybear
{

WaveFile::WaveFile()
{
}

WaveFile::~WaveFile()
{
}

void WaveFile::LoadFromFile(wchar_t *filename)
{
  std::wifstream file(filename, std::ios::ate | std::ios::binary);
  if(!file.is_open())
  {
    assert(false && "Unable to open the specified audio file");
  }
  std::wifstream::pos_type size = file.tellg() * (sizeof(wchar_t) / sizeof(char));
  wchar_t *memblock = DBG_NEW wchar_t[size];
  file.seekg(0, std::ios::beg);
  file.read(memblock, size);

  // file opened and read!
  memcpy(WaveFile::m_waveHeader, &memblock, sizeof(WaveHeader));

  // header data should be satisfactory now, make sanity checks!

  file.close();
  delete[] memblock;
}

} // namespace shinybear