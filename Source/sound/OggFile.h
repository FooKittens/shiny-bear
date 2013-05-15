#ifndef SHINYBEAR_OGGFILE_H
#define SHINYBEAR_OGGFILE_H

#include "resource\types\ISoundResource.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <vorbis\vorbisenc.h>
#include <vorbis\vorbisfile.h>

namespace shinybear
{
#pragma pack(push, 1)
struct OggHeader
{

};
#pragma pack(pop)

class OggFile : public ISoundResource
{
public:
  static OggFile *LoadFromFile(wchar_t *filename, SoundProvider*);

private:
  OggFile();
  OggHeader m_oggHeader;

  // Ogg Callbacks
  static size_t OggReadCallback(void*, size_t, size_t, void*);
  static int OggSeekCallback(void*, ogg_int64_t, int);
  static int OggCloseCallback(void*);
  static long OggTellCallback(void*);
};

} // namespace shinybear

#endif // SHINYBEAR_OGGFILE_H