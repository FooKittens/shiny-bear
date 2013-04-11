#ifndef TESTAPP_H
#define TESTAPP_H

#include "Framework\DXApp.h"
#include "Framework\InputManager.h"
#include <windows.h>
#include <d3dx9.h>


struct IDirect3DTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DVertexDeclaration9;
namespace shinybear { struct DisplayMode; struct MultiSampleMode; }

class TestApp : public framework::DXApp {

public:
  TestApp(HINSTANCE hInst, const SIZE &size);
  ~TestApp();

  void Initialize();

protected:

  void Update(double dt);
  void DrawScene();

  void OnDeviceLost();
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

  UINT m_dpmc;
  UINT m_msac;
  UINT m_currentDisplayIndex;
  UINT m_currentMSAAIndex;
  shinybear::DisplayMode *m_pDisplayModes;
  shinybear::MultiSampleMode *m_pMSAAModes;
  framework::KeyboardState m_prevState;
  framework::KeyboardState m_newState;
};

#endif