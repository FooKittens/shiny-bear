#ifndef TESTAPP_H
#define TESTAPP_H

#include "Framework\DXApp.h"
#include <windows.h>
#include <d3dx9.h>


struct IDirect3DTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DVertexDeclaration9;

class TestApp : public framework::DXApp {

public:
  TestApp(HINSTANCE hInst, const SIZE &size);
  ~TestApp();

  void Initialize();

protected:

  void Update(double dt);
  void DrawScene();
private:
  void CreateVertexBuffer();
  void CreateIndexBuffer();
  void CreateEffect();
  void LoadTexture();

  float camX, camY, camZ, camYRot;
  IDirect3DVertexBuffer9 *m_pVBuffer;
  IDirect3DIndexBuffer9 *m_pIBuffer;
  IDirect3DVertexDeclaration9 *m_pDecl;
  IDirect3DTexture9 *m_pTexture;
  D3DXMATRIX m_matWorld, m_matView, m_matProjection;
  ID3DXEffect *m_pEffect;
  D3DXHANDLE m_hFxWVP;
  
};

#endif