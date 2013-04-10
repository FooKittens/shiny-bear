#ifndef SBUTIL_H
#define SBUTIL_H

#define ReleaseCOM(X) if(X) X->Release()

#define HR(X) if(FAILED(X)) \
  MessageBox(NULL, TEXT(#X), TEXT("HR ERROR"), MB_OK | MB_ICONERROR)

#endif