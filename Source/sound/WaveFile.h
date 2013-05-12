#ifndef SHINYBEAR_WAVEFILE_H
#define SHINYBEAR_WAVEFILE_H

#include <dsound.h>

namespace shinybear
{

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

class WaveFile
{
public:
  WaveFile();
  ~WaveFile();

  void LoadFromFile(wchar_t *filename);

private:
  WaveHeader *m_waveHeader;
  IDirectSoundBuffer8 *m_secondaryBuffer1;
};

} // namespace shinybear

#endif // SHINYBEAR_WAVEFILE_H