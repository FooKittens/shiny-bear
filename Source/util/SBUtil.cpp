#include "SBUtil.h"
#include <Windows.h>
#include <wchar.h>
#include <cassert>
#include <crtdbg.h>

namespace shinybear
{

// Checks if a file exists with the input path.
// We need to prepend the string with an escape-sequence
// to allow for paths longer than MAX_PATH.
bool FileExists(const wchar_t *pPath, size_t len)
{
  // +5 is size adjustment to contain the prefix for extra long paths.
  wchar_t *pBuffer = DBG_NEW wchar_t[len + 5];

  wsprintfW(pBuffer, L"\\\\?\\%s", pPath);

  DWORD attribs = GetFileAttributesW(pBuffer);

  delete[] pBuffer;
  
  return (attribs != INVALID_FILE_ATTRIBUTES &&
    !(attribs & FILE_ATTRIBUTE_DIRECTORY));
}

// Prints a char string into a wide buffer.
void ToWideChar(const char *str, size_t len,
  wchar_t *pTarget, size_t targetSize)
{
  
  // if 0 is passed for len it should mean the string is null terminated.
  if(len == 0)
  {
    len = strlen(str);
  }

  // Make sure the buffer is large enough to contain the string.
  assert((len + 1) <= targetSize && "TargetBuffer is too small!");

  // Print the string into the wide buffer.
  wsprintfW(pTarget, L"%s", str);
}

size_t GetExecutablePath(wchar_t **ppBuffer, size_t *pSize) 
{

  // First size to try.
  const static size_t kFSize = 256;

  size_t retSize = -1;

  if(*pSize == 0)
  {
    *pSize = kFSize;
    *ppBuffer = DBG_NEW wchar_t[*pSize];
  }

  retSize = GetModuleFileNameW(GetModuleHandleW(NULL), *ppBuffer, *pSize);

  // Calculate how much space is missing, if any.
  size_t diff = (*pSize) - retSize;

  // If there is space missing, we re-allocate the buffer to be large enough.
  if(diff > 0)
  {
    // Adjust the size parameter.
    *pSize += diff;
    
    // Free the previously allocated buffer.
    delete[] *ppBuffer;

    // Allocate a buffer large enough to hold the path.
    *ppBuffer = DBG_NEW wchar_t[*pSize];

    // Attempt to get the path.
    retSize = GetModuleFileNameW(GetModuleHandleW(NULL), *ppBuffer, *pSize);

    // This should never fail unless calculations about strings are incorrect.
    assert((retSize + 1) <= (*pSize) && "BufferCalculation invalid!");
  }

  // Put the path to the directory in the buffer.
  size_t pathStrLen = GetFileDirectoryPath(*ppBuffer, *ppBuffer, pSize);

  assert(pathStrLen != 0 && "Invalid Buffer allocation size!");

  return pathStrLen;
}

size_t GetFileDirectoryPath(const wchar_t *pFilePath, wchar_t *pBuffer,
  size_t *bSize)
{

  // Retrieve the length of the filepath.
  size_t fpSize = wcslen(pFilePath);

  // Holds offset from end until the last backslash.
  size_t offset = 0;

  // Find the last backslash.
  while(pFilePath[fpSize - offset] != '\\')
  {
    ++offset;
  }

  // Check if the buffer is large enough to hold the string.
  if(fpSize - offset <= *bSize) 
  {
    wcsncpy(pBuffer, pFilePath, fpSize - offset + 1);
    pBuffer[fpSize - offset + 1] = '\0';

    // return the length of the string.
    return fpSize - offset + 1;
  }
  else
  {
    *bSize = fpSize - offset + 1;
    return 0;
  }
}


size_t GetAbsolutePath(const wchar_t *pRelativePath, wchar_t **ppBuffer)
{

  size_t bSize = 128;
  wchar_t *pExePath = DBG_NEW wchar_t[bSize];

  // Get the location of the executable.
  size_t exePathLen = GetExecutablePath(&pExePath, &bSize);

  // Calculate length of the relativePath
  size_t relativeLen = wcslen(pRelativePath);

  // Allocate a buffer that can hold both strings.
  *ppBuffer = DBG_NEW wchar_t[exePathLen + relativeLen + 2];

  // Copy the executable path to the buffer.
  wcsncpy(*ppBuffer, pExePath, exePathLen);

  delete[] pExePath;

  (*ppBuffer)[exePathLen] = '\0';

  // Append the relative path to the absolute path.
  wcsncat(*ppBuffer, pRelativePath, relativeLen);

  (*ppBuffer)[exePathLen + relativeLen + 1] = '\0';

  return wcslen(*ppBuffer);  
}

// Prints a formatted string to the outputwindow.
void OutputDbgFormat(const char *format, ...)
{
  char buffer[4096];
  va_list args;
  va_start(args, format);
  vsprintf_s(buffer, format, args);
  va_end(args);
  OutputDebugString(buffer);
  OutputDebugString("\n");
}

} // namespace shinybear