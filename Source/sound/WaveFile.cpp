#include "sound\WaveFile.h"
#include "sound\SoundProvider.h"
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
  memset(&m_waveHeader, 0, sizeof(WaveHeader));
}

WaveFile *WaveFile::LoadFromFile(wchar_t *filename, SoundProvider *pSoundProvider)
{
  WaveFile *pResult = DBG_NEW WaveFile();
  pResult->m_pSoundProvider = pSoundProvider;
  WaveHeader &wh = pResult->m_waveHeader;

  // Open file stream
  std::ifstream file(filename, std::ios::binary);
  if(!file.is_open())
  {
    assert(false && "Unable to open the specified audio file");
  }
  
  // Read file header
  size_t bufSz = sizeof(WaveHeader);
  file.read(reinterpret_cast<char*>(&wh), bufSz);

  // Make sure it's a wave file
  if((wh.chunkId[0] != 'R') || (wh.chunkId[1] != 'I') || 
	   (wh.chunkId[2] != 'F') || (wh.chunkId[3] != 'F'))
	{
		assert(false && "Bad wave file! (not RIFF chunk format)");
  }
  if((wh.format[0] != 'W') || (wh.format[1] != 'A') ||
	   (wh.format[2] != 'V') || (wh.format[3] != 'E'))
	{
		assert(false && "Bad wave file! (not WAVE format)");
	}
  if((wh.subChunkId[0] != 'f') || (wh.subChunkId[1] != 'm') ||
	   (wh.subChunkId[2] != 't') || (wh.subChunkId[3] != ' '))
	{
		assert(false && "Bad wave file! (not fmt sub chunk format)");
	}
  if(wh.audioFormat != WAVE_FORMAT_PCM)
	{
		assert(false && "Bad wave file! (not PCM format)");
	}

  // All is good, so we load the actual data!
  bufSz = pResult->m_waveHeader.dataSize;
  char *pPCMData = DBG_NEW char[bufSz];
  file.read(pPCMData, bufSz);
  file.close();

  WAVEFORMATEX waveFormat;
  waveFormat.cbSize = 0;
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nChannels = wh.numChannels;
  waveFormat.nBlockAlign = wh.blockAlign;
  waveFormat.nSamplesPerSec = wh.sampleRate;
  waveFormat.nAvgBytesPerSec = wh.blockAlign * wh.sampleRate;
  waveFormat.wBitsPerSample = wh.bitsPerSample;
  
  DSBUFFERDESC bufDesc = *CreateBufferDescription(&waveFormat, wh.dataSize);

  // ... and create the sound buffer with the description above
  pResult->m_pSecondaryBuffer = CreateSecondaryBuffer(
    pPCMData, &bufDesc, pSoundProvider);
  
  return pResult;
}

} // namespace shinybear