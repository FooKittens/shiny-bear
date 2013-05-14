#ifndef SHINYBEAR_WAVEFILE_H
#define SHINYBEAR_WAVEFILE_H

#include "resource\types\ISoundResource.h"

namespace shinybear
{
#pragma pack(push, 1)
struct WaveHeader
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};
#pragma pack(pop)

class WaveFile : public ISoundResource
{
public:
  static WaveFile *LoadFromFile(wchar_t *filename, SoundProvider*);

private:
  WaveFile();
  WaveHeader m_waveHeader;
};

} // namespace shinybear

#endif // SHINYBEAR_WAVEFILE_H