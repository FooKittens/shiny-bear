#include "graphics\VertexManager.h"
#include "base\system\GraphicsProvider.h"
#include "resource\ResourceManager.h"
#include "util\SBUtil.h"

#include <cassert>


namespace shinybear
{
// Default to false.
bool VertexManager::isInitialized = false;
VertexManager *VertexManager::pSingleton = nullptr;

void VertexManager::Initialize(GraphicsProvider *pProvider)
{
  assert(!isInitialized && "VertexManager already initialized!");

  pSingleton = DBG_NEW VertexManager(pProvider);

  // Create built-in vertex declarations.
  pSingleton->CreateDefaultDecl();
}

void VertexManager::Cleanup()
{
  delete pSingleton;
}

VertexManager::VertexManager(GraphicsProvider *pProvider)
{
  m_pProvider = pProvider;
}

VertexManager::~VertexManager()
{
  auto it = m_declCollection.begin();
  auto end = m_declCollection.end();

  while(it != end)
  {
    delete it++->second;
  }

  m_declCollection.clear();
}

void VertexManager::CreateDefaultDecl()
{
  VertexDeclaration *pVP = DBG_NEW VertexDeclaration(m_pProvider);
  pVP->AddElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_POSITION, 0);

  // Vertex-position-normal-color
  VertexDeclaration *pVPNC = DBG_NEW VertexDeclaration(m_pProvider);
  pVPNC->AddElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_POSITION, 0);
  pVPNC->AddElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_NORMAL, 0);
  pVPNC->AddElement(VertexDataType::VDT_FLOAT4, VertexUsageType::VUT_COLOR, 0);

  // Vertex-position-color
  VertexDeclaration *pVPC = DBG_NEW VertexDeclaration(m_pProvider);
  pVPC->AddElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_POSITION, 0);
  pVPC->AddElement(VertexDataType::VDT_FLOAT4, VertexUsageType::VUT_COLOR, 0);
  
  // Vertex position-texture.
  VertexDeclaration *pVPTx = DBG_NEW VertexDeclaration(m_pProvider);
  pVPTx->AddElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_POSITION, 0);
  pVPTx->AddElement(VertexDataType::VDT_FLOAT2, VertexUsageType::VUT_TEXCOORDS, 0);

  // Vertex Position-normal-texcoords.
  VertexDeclaration *pVPNTx = DBG_NEW VertexDeclaration(m_pProvider);
  pVPNTx->AddElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_POSITION, 0);
  pVPNTx->AddElement(VertexDataType::VDT_FLOAT3, VertexUsageType::VUT_NORMAL, 0);
  pVPNTx->AddElement(VertexDataType::VDT_FLOAT2, VertexUsageType::VUT_TEXCOORDS, 0);

  // Vertex Deferred-Fullscreen-target (Used for rendering full screen quads.)
  VertexDeclaration *pVDFT = DBG_NEW VertexDeclaration(m_pProvider);
  
  pVDFT->AddElement(VertexDataType::VDT_FLOAT4, VertexUsageType::VUT_POSITION, 0);
  pVDFT->AddElement(VertexDataType::VDT_FLOAT2, VertexUsageType::VUT_TEXCOORDS, 0);

  // For a fullscreen quad with neither textures or colors.
  VertexDeclaration *pVDF = DBG_NEW VertexDeclaration(m_pProvider);
  pVDF->AddElement(VertexDataType::VDT_FLOAT4, VertexUsageType::VUT_POSITION, 0);

  // Make sure it recreates its resources.
  pVP->OnDeviceReset(m_pProvider);
  pVPNC->OnDeviceReset(m_pProvider);
  pVPC->OnDeviceReset(m_pProvider);
  pVPTx->OnDeviceReset(m_pProvider);
  pVPNTx->OnDeviceReset(m_pProvider);
  pVDFT->OnDeviceReset(m_pProvider);
  pVDF->OnDeviceReset(m_pProvider);

  RegisterVertexDeclaration(VertexDecl::VD_POSITION, pVP);
  RegisterVertexDeclaration(VertexDecl::VD_POSITION_COLOR_NORMAL, pVPNC);
  RegisterVertexDeclaration(VertexDecl::VD_POSITION_COLOR, pVPC);
  RegisterVertexDeclaration(VertexDecl::VD_POSITION_TEXTURE, pVPTx);
  RegisterVertexDeclaration(VertexDecl::VD_POSITION_NORMAL_TEXTURE, pVPNTx);
  RegisterVertexDeclaration(VertexDecl::VD_DFT, pVDFT);
  RegisterVertexDeclaration(VertexDecl::VD_DF, pVDF);

  ResourceManager::RegisterResource(pVP, "VertexP");
  ResourceManager::RegisterResource(pVPNC, "VertexPNC");
  ResourceManager::RegisterResource(pVPC, "VertexPC");
  ResourceManager::RegisterResource(pVPTx, "VertexPTx");
  ResourceManager::RegisterResource(pVPNTx, "VertexPNTx");
  ResourceManager::RegisterResource(pVDFT, "VertexDFT");
  ResourceManager::RegisterResource(pVDF, "VertexDF");
}

VertexDeclaration * const VertexManager::GetVertexDeclaration(VertexDecl::E type)
{
  return pSingleton->SafeGetDecl(type);
}

void VertexManager::RegisterVertexDeclaration(VertexDecl::E type, VertexDeclaration *pDecl)
{
  pSingleton->SafeRegDecl(type, pDecl);
}

VertexDeclaration * const VertexManager::SafeGetDecl(VertexDecl::E type)
{
  auto declPair = m_declCollection.find(type);
  if(declPair == m_declCollection.end())
  {
    return nullptr;
  }
  
  return declPair->second;
}

void VertexManager::SafeRegDecl(VertexDecl::E type, VertexDeclaration *pDecl)
{
 assert(!DeclExists(type) && "VertexDecl already exists!");

 m_declCollection.insert(VertexMapping(type, pDecl));
}

bool VertexManager::DeclExists(VertexDecl::E type)
{
  auto declPair = m_declCollection.find(type);
  return (declPair != m_declCollection.end());
}

} // namespace shinybear