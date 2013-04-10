#include "TestApp.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Framework\InputManager.h"
#include "Framework\DXUtil.h"


using namespace framework;

struct VertexP {
  VertexP(float x, float y, float z, float a,
    float r, float g, float b):position(x, y, z),color(r,g,b,a) { }

  VertexP(D3DXVECTOR3 pos, D3DXVECTOR3 col) :position(pos),color(col) { }

  D3DXVECTOR3 position;
  D3DXVECTOR4 color;
};

struct VertexPT {

  VertexPT() { }
  VertexPT(float x, float y, float z, float u, float v):position(x, y, z), texCoords(u, v) { }

  D3DXVECTOR3 position;
  D3DXVECTOR2 texCoords;
};

TestApp::TestApp(HINSTANCE hInst, const SIZE &size)
  :DXApp(hInst, size) {

}

TestApp::~TestApp() {
	RELEASECOM(m_pVBuffer);
	RELEASECOM(m_pIBuffer);
	RELEASECOM(m_pDecl);
	RELEASECOM(m_pTexture);
	RELEASECOM(m_pEffect);
}


void TestApp::Initialize() {
  DXApp::Initialize();

  IDirect3DDevice9 *pDevice = GetDevice();

  CreateVertexBuffer();
  CreateIndexBuffer();

  D3DVERTEXELEMENT9 elems[] = {
    { 0, 0, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0 }, 
    D3DDECL_END()
  };
  
  HR(pDevice->CreateVertexDeclaration(elems, &m_pDecl));
  

  D3DXMatrixIdentity(&m_matWorld);

  D3DXMatrixPerspectiveFovLH(
    &m_matProjection,
    0.25f * 3.141592,
    (float)GetSize().cx / (float)GetSize().cy,
    1.0f,
    100.0f
  );

  camX = camZ = 0;
  camYRot = -3.141592 / 2;
  camY = 1.0f;

  LoadTexture();
  CreateEffect();

  D3DXHANDLE hFxText = m_pEffect->GetParameterByName(0, "gText");
  m_pEffect->SetTexture(hFxText, m_pTexture);

  m_hFxWVP = m_pEffect->GetParameterByName(0, "gWVP");

}

void TestApp::CreateEffect() {
  ID3DXBuffer *errors = nullptr;
  D3DXCreateEffectFromFile(
    GetDevice(),
    "SimpleShader.fx",
    0,
    0,
    D3DXSHADER_DEBUG,
    NULL,
    &m_pEffect,
    &errors
  );

  if(errors) {
    MessageBox(0, (char*)errors->GetBufferPointer(), 0, MB_OK | MB_ICONERROR);
  }
}

void TestApp::LoadTexture() {

  HR(D3DXCreateTextureFromFile(GetDevice(), "testAlpha.png", &m_pTexture));

}

void TestApp::CreateVertexBuffer() {

  IDirect3DDevice9 *pDevice = GetDevice();
  HR(pDevice->CreateVertexBuffer(
    sizeof(VertexPT) * 8,
    0,
    0,
    D3DPOOL_MANAGED,
    &m_pVBuffer,
    0
  ));

  VertexPT *pVertices;
  HR(m_pVBuffer->Lock(0, 0, (void**)&pVertices, 0));

  pVertices[0] = VertexPT(-0.5f, +0.5f, -0.5f, 0, 0);
  pVertices[1] = VertexPT(+0.5f, +0.5f, -0.5f, 1, 0);
  pVertices[2] = VertexPT(+0.5f, -0.5f, -0.5f, 1, 1);
  pVertices[3] = VertexPT(-0.5f, -0.5f, -0.5f, 0, 1);
  pVertices[4] = VertexPT(+0.5f, +0.5f, +0.5f, 0, 0);
  pVertices[5] = VertexPT(-0.5f, +0.5f, +0.5f, 1, 0);
  pVertices[6] = VertexPT(-0.5f, -0.5f, +0.5f, 1, 1);
  pVertices[7] = VertexPT(+0.5f, -0.5f, +0.5f, 0, 1);

  HR(m_pVBuffer->Unlock());
}

void TestApp::CreateIndexBuffer() {

  IDirect3DDevice9 *pDevice = GetDevice();

  HR(pDevice->CreateIndexBuffer(
    sizeof(int) * 36,
    0,
    D3DFMT_INDEX32,
    D3DPOOL_MANAGED,
    &m_pIBuffer,
    0
  ));

  int *pIndices;
  HR(m_pIBuffer->Lock(0, 0, (void**)&pIndices, 0));

  // Front face
  pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
  pIndices[3] = 2; pIndices[4] = 3; pIndices[5] = 0;
  
  // Top face
  pIndices[6] = 5; pIndices[7] = 4; pIndices[8] = 1;
  pIndices[9] = 1; pIndices[10] = 0; pIndices[11] = 5;

  // Back face
  pIndices[12] = 4; pIndices[13] = 5; pIndices[14] = 6;
  pIndices[15] = 6; pIndices[16] = 7; pIndices[17] = 4;

  // Bottom face
  pIndices[18] = 3; pIndices[19] = 2; pIndices[20] = 7;
  pIndices[21] = 7; pIndices[22] = 6; pIndices[23] = 3;

  // Left face
  pIndices[24] = 5; pIndices[25] = 0; pIndices[26] = 3;
  pIndices[27] = 3; pIndices[28] = 6; pIndices[29] = 5;

  // Right face
  pIndices[30] = 1; pIndices[31] = 4; pIndices[32] = 7;
  pIndices[33] = 7; pIndices[34] = 2; pIndices[35] = 1;

  HR(m_pIBuffer->Unlock());
}

void TestApp::Update(double dt) {

  camYRot += 1.0f * dt;

  if(camYRot > 3.141592 * 2) {
    camYRot = 0;
  }

  camX = cosf(camYRot) * 5;
  camZ = sinf(camYRot) * 5;

  D3DXVECTOR3 eye(camX, camY, camZ);
  D3DXVECTOR3 at(0, 0, 0);
  D3DXVECTOR3 up(0, 1, 0);
  D3DXMatrixLookAtLH(&m_matView, &eye, &at, &up);
}

void TestApp::DrawScene() {
  IDirect3DDevice9 *pDevice = GetDevice();
  HR(pDevice->BeginScene());
  HR(pDevice->Clear(
    0,
    0,
    D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
    0x88888888,
    1.0f,
    0
  ));

  D3DVIEWPORT9 vp;
  vp.Height = GetSize().cy;
  vp.Width = GetSize().cx;
  vp.MaxZ = 1.0f;
  vp.MinZ = 0.0f;
  vp.X = 0;
  vp.Y = 0;
  pDevice->SetViewport(&vp);

  HR(pDevice->SetStreamSource(0, m_pVBuffer, 0, sizeof(VertexPT)));
  HR(pDevice->SetIndices(m_pIBuffer));
  HR(pDevice->SetVertexDeclaration(m_pDecl));

  //HR(pDevice->SetTransform(D3DTS_WORLD, m_pMatWorld));
  //HR(pDevice->SetTransform(D3DTS_VIEW, m_pMatView));
  //HR(pDevice->SetTransform(D3DTS_PROJECTION, m_pMatProjection));
  //HR(pDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_FILLMODE, D3DFILL_WIREFRAME));
  //HR(pDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW));
  
  HR(pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
  HR(pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
  HR(pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA));
  HR(pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD));

  D3DXMATRIX m = m_matWorld * m_matView * m_matProjection; 
  
  m_pEffect->SetMatrix(m_hFxWVP, &m);
  D3DXHANDLE hTech = m_pEffect->GetTechniqueByName("SimpleTechnique");
  
  m_pEffect->SetTechnique(hTech);

  UINT numPasses = 0;
  HR(m_pEffect->Begin(&numPasses, 0));
  for(int i = 0; i < numPasses; ++i) {
    HR(m_pEffect->BeginPass(i));
    HR(pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));  
    HR(m_pEffect->EndPass());
  }
  HR(m_pEffect->End());
  

  DrawFpsText();
  pDevice->EndScene();
  pDevice->Present(0, 0, 0, 0);
}