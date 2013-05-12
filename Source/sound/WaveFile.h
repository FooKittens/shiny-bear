#ifndef SHINYBEAR_WAVEFILE_H
#define SHINYBEAR_WAVEFILE_H

#include "resource\types\ISoundResource.h"
#include "sound\SoundProvider.h"

#include <dsound.h>

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
#pragma pop

class WaveFile : public ISoundResource
{
public:
  ~WaveFile();

  static WaveFile *LoadFromFile(wchar_t *filename, SoundProvider*);
  void Play();

private:
  WaveFile();
  WaveHeader m_waveHeader;
  char *m_pPCMData;
  IDirectSoundBuffer8 *m_pSecondaryBuffer;
};

} // namespace shinybear

#endif // SHINYBEAR_WAVEFILE_H