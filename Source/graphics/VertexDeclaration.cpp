#include "graphics\VertexDeclaration.h"
#include "base\system\GraphicsProvider.h"
#include "util\SBUtil.h"

#include <cassert>

namespace shinybear
{

const D3DVERTEXELEMENT9 VertexDeclaration::kEndElement = 
{ 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 };


VertexDeclaration::VertexDeclaration(GraphicsProvider *pProvider)
{
  m_pDecl = nullptr;
  m_pProvider = pProvider;
}

VertexDeclaration::~VertexDeclaration()
{
  RELEASECOM(m_pDecl);
}

void VertexDeclaration::Activate()
{
  assert(m_pDecl && "Vertex Declaration not initialized!");

  m_pProvider->GetDevice()->SetVertexDeclaration(m_pDecl);
}

void VertexDeclaration::OnDeviceLost()
{
  RELEASECOM(m_pDecl);
}

void VertexDeclaration::OnDeviceReset(GraphicsProvider *pProvider)
{
  RELEASECOM(m_pDecl);
  D3DVERTEXELEMENT9 *pElements = DBG_NEW D3DVERTEXELEMENT9[m_elements.size() + 1];

  UINT offset = 0;
  for(size_t i = 0; i < m_elements.size(); ++i)
  {
    pElements[i].Method = D3DDECLMETHOD_DEFAULT;
    pElements[i].Offset = offset;
    pElements[i].Stream = 0;
    pElements[i].Usage = GetVertexUsageType(m_elements[i].vertexUsageType);
    pElements[i].UsageIndex = m_elements[i].slot;
    pElements[i].Type = GetVertexType(m_elements[i].vertexDataType);
    offset += CalcOffset(m_elements[i].vertexDataType);
  }
  pElements[m_elements.size()] = kEndElement;

  HR(m_pProvider->GetDevice()->CreateVertexDeclaration(pElements, &m_pDecl));
  delete[] pElements;
}

D3DDECLUSAGE VertexDeclaration::GetVertexUsageType(VertexUsageType::Enum type)
{
  switch(type)
  {
  case VertexUsageType::VUT_POSITION: return D3DDECLUSAGE_POSITION;
  case VertexUsageType::VUT_COLOR: return D3DDECLUSAGE_COLOR;
  case VertexUsageType::VUT_TEXCOORDS: return D3DDECLUSAGE_TEXCOORD;
  case VertexUsageType::VUT_NORMAL: return D3DDECLUSAGE_NORMAL;
  default:
    assert(false && "Invalid usage type!");
    return D3DDECLUSAGE_POSITION;
  }
}

D3DDECLTYPE VertexDeclaration::GetVertexType(VertexDataType::Enum type)
{
  switch(type)
  {
  case VertexDataType::VDT_FLOAT:   return D3DDECLTYPE_FLOAT1;
  case VertexDataType::VDT_FLOAT2:  return D3DDECLTYPE_FLOAT2;
  case VertexDataType::VDT_FLOAT3:  return D3DDECLTYPE_FLOAT3;
  case VertexDataType::VDT_FLOAT4:  return D3DDECLTYPE_FLOAT4;
  default:
    assert(false && "Invalid usage type!");
    return D3DDECLTYPE_UNUSED;
  }
}

UINT VertexDeclaration::CalcOffset(VertexDataType::Enum type)
{
  switch(type)
  {
  case VertexDataType::VDT_FLOAT:   return sizeof(FLOAT);
  case VertexDataType::VDT_FLOAT2:  return sizeof(FLOAT) * 2;
  case VertexDataType::VDT_FLOAT3:  return sizeof(FLOAT) * 3;
  case VertexDataType::VDT_FLOAT4:  return sizeof(FLOAT) * 4;
  default:
    assert(false && "Invalid usage type!");
    return 0;
  }
}

} // namespace shinybear

