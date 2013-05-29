#include "sound\OggFile.h"
#include "util\SBUtil.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <vorbis\vorbisfile.h>

namespace shinybear
{

OggFile::OggFile()
{
  memset(&m_oggHeader, 0, sizeof(OggHeader));
}

OggFile *OggFile::LoadFromFile(wchar_t *filename, SoundProvider *pSoundProvider)
{  
  OggFile *pResult = DBG_NEW OggFile;
  pResult->m_pSoundProvider = pSoundProvider;
  OggVorbis_File *oggFile = DBG_NEW OggVorbis_File;

  // Open the file
  std::ifstream file(filename, std::ios::binary);
  ov_callbacks ocb;
  ocb.read_func = &OggReadCallback;
  ocb.seek_func = &OggSeekCallback;
  ocb.close_func = &OggCloseCallback;
  ocb.tell_func  = &OggTellCallback;
  if(file.is_open())
  {
    long errorMsg = ov_open_callbacks(&file, oggFile, NULL, 0, ocb);
    if(errorMsg < 0)
    {
      assert(false && "The file does not appear to be an Ogg bitstream!");
    }
  }

  // Get the info
  const int kGetBitstreamData = -1;
  vorbis_info *oggInfo = ov_info(oggFile, kGetBitstreamData);
  int channelCount = oggInfo->channels;
  long sampleRate = oggInfo->rate;
  long sampleCount = ov_pcm_total(oggFile, kGetBitstreamData);
  if(sampleCount == OV_EINVAL)
  {
    assert(false && "Ogg bitstream is unseekable!");
  }
  
  // Read the pcm data
  // 2 is the modifier for the 16 bit samples vs 8 bit chars
  char *pPCMData = DBG_NEW char[sampleCount * channelCount * 2];
  char tempBuffer[4096];
  bool eof = false;
  int current_section = 0;
  long totalReadBytes = 0;
  while(!eof)
  {
    const int kLittleEndian = 0; // 1 for big endian
    const int k16bitSamples = 2; // 1 for 8 bit samples
    const int kSignedData = 1; // 0 for unsigned data
    
    long readBytes = ov_read(oggFile, tempBuffer, sizeof(tempBuffer),
      kLittleEndian, k16bitSamples, kSignedData, &current_section
    );
    totalReadBytes += readBytes;
    if (readBytes == 0)
    {
      // EOF
      eof = true;
    }
    else if (readBytes < 0)
    {
      assert(false && "Error in ogg read input stream!");
    }
    else
    {
      // we don't bother dealing with sample rate changes, etc, but you'll have to
      memcpy((void*)&(pPCMData[totalReadBytes - readBytes]), tempBuffer, readBytes);
    }
  }

  // Register the sound buffer
  WAVEFORMATEX waveFormat;
  waveFormat.cbSize = 0;
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nChannels = channelCount;
  waveFormat.nBlockAlign = 2 * waveFormat.nChannels;
  waveFormat.nSamplesPerSec = sampleRate;
  waveFormat.nAvgBytesPerSec = waveFormat.nBlockAlign * waveFormat.nSamplesPerSec;
  waveFormat.wBitsPerSample = 16;

  DSBUFFERDESC bufDesc = *CreateBufferDescription(&waveFormat, totalReadBytes);

  pResult->m_pSecondaryBuffer = CreateSecondaryBuffer(
    pPCMData, &bufDesc, pSoundProvider);

  // Clean up the mess
  ov_clear(oggFile);
  delete oggFile;
  return pResult;
}

// Ogg Callbacks
size_t OggFile::OggReadCallback(void *ptr, size_t size, size_t nmemb, void *datasource)
{
  std::ifstream *file = (std::ifstream*)datasource;
  unsigned int nBytesToCopy = size * nmemb;
  
  // Check remaining file size
  std::streampos curPos = file->tellg();
  file->seekg(0, file->end);
  unsigned int nBytesRemaining = file->tellg() - curPos;

  // Keep reading from where we were
  file->seekg(curPos);
  if(nBytesToCopy > nBytesRemaining)
    nBytesToCopy = nBytesRemaining;
  file->read((char*)ptr, nBytesToCopy);

  unsigned int pos = file->tellg();
  return nBytesToCopy;
}

int OggFile::OggSeekCallback(void *datasource, ogg_int64_t offset, int whence)
{
  std::ifstream *file = (std::ifstream*)datasource;
  switch(whence)
  {
  case SEEK_SET:
    file->seekg(offset, file->beg);
    break;
  case SEEK_CUR:
    file->seekg(offset, file->tellg());
    break;
  case SEEK_END:
    file->seekg(offset, file->end);
    break;
  default:
    assert(false && "External Ogg Vorbis library error!");
  }
  unsigned long test = file->tellg();
  // Check for out of bounds seek pointer
  if(file->tellg() < 0)
  {
    file->seekg(0);
    return -1;
  }
  else
  {
    std::streampos curPos = file->tellg();
    file->seekg(0, file->end);
    std::streampos endPos = file->tellg();
    if(curPos > endPos)
    {
      file->seekg(0, file->end);
      return -1;
    }
    file->seekg(curPos, file->beg);
  }
  
  return 0;
}

int OggFile::OggCloseCallback(void *datasource)
{
  std::ifstream *file = (std::ifstream*)datasource;
  file->close();
  file = nullptr;
  return 0;
}

long OggFile::OggTellCallback(void *datasource)
{
  std::ifstream *file = (std::ifstream*)datasource;
  unsigned long test = file->tellg();
  return file->tellg();
}

} // namespace shinybear