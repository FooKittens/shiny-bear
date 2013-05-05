#ifndef SBUTIL_H
#define SBUTIL_H

#include <cstdarg>
#include <cstdio>
#include <Windows.h>
#include <crtdbg.h>

#define RELEASECOM(X) if(X) { X->Release(); X = nullptr; }

#define DEBUG

#if defined(DEBUG) | defined(_DEBUG)

// Error checking macro.
#define HR(X) if(FAILED(X)) \
  MessageBox(NULL, TEXT(#X), TEXT("HR ERROR"), MB_OK | MB_ICONERROR)

// Memory leak detection.
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define HR(X) X
#define DBG_NEW new
#endif

 

// Macro to make sure deleted pointers are set to 0
#define SAFEDELETE(X) delete X; X = 0

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

// Function for checking if a file exists
namespace shinybear
{

// Used to determine if a file exists
bool FileExists(const wchar_t *pPath, size_t len);

void ToWideChar(const char *str, size_t len, wchar_t *pTarget, size_t targetSize);

// Retrieves the path of the executable.
// ppBuffer : pointer to an allocated buffer,
//  will be realllocated if not large enough.
// pSize : size of the buffer, will be changed if the buffer is relocated.
// returns : length excluding the null-termination char.
size_t GetExectuablePath(wchar_t **ppBuffer, size_t *pSize);

// Puts the directory path in pBuffer if bSize is large enough.
// Returns the length of the string.
// if bSize is not large enough it returns 0 and puts the required
// size in bSize.
size_t GetFileDirectoryPath(const wchar_t *pFilePath, wchar_t *pBuffer,
  size_t *bSize);

// Takes a relative path (from the executable) and converts into
// an absolute path, returns the size of the new buffer, which
// must be freed by the caller.
size_t GetAbsolutePath(const wchar_t *pRelativePath, wchar_t **ppBuffer);

// Writes to the output window using a formatted string.
void OutputDbgFormat(const char *format, ...);

} // namespace shinybear
#endif