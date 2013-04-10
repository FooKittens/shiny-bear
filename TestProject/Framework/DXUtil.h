#ifndef DXUTIL_H
#define DXUTIL_H

#define RELEASECOM(X) if(X) X->Release()
#define HR(X) if(FAILED(X)) MessageBox(NULL, TEXT(#X), TEXT("ERROR"), MB_OK | MB_ICONERROR)



#endif