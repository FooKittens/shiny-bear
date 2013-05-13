#ifndef SHINYBEAR_VERTEXDECLARATION_H
#define SHINYBEAR_VERTEXDECLARATION_H

#include "resource\types\IGraphicsResource.h"

#include <d3d9.h>
#include <vector>

namespace shinybear { class GraphicsProvider; }

namespace shinybear
{

namespace VertexUsageType
{
  enum Enum
  {
    VUT_POSITION = 1,
    VUT_COLOR = 2,
    VUT_TEXCOORDS = 3,
    VUT_NORMAL = 4,
  };
}

namespace VertexDataType
{
  enum Enum
  {
    VDT_FLOAT = 1,
    VDT_FLOAT2 = 2,
    VDT_FLOAT3 = 3,
    VDT_FLOAT4 = 4,
  };
}

struct VertexElement
{
  VertexElement(VertexDataType::Enum dataType,
    VertexUsageType::Enum usage, UINT slot)
    :vertexUsageType(usage), vertexDataType(dataType), slot(slot) { }

  VertexUsageType::Enum vertexUsageType;
  VertexDataType::Enum vertexDataType;
  UINT slot;
};


class VertexDeclaration : public IGraphicsResource
{
public:
  VertexDeclaration(GraphicsProvider *pProvider);
  ~VertexDeclaration();

  void AddElement(const VertexElement &elem);
  void AddElement(VertexDataType::Enum vdt, VertexUsageType::Enum vut, UINT slot);
  void RemoveElement(UINT index);

  void Activate();

  void OnDeviceLost();
  void OnDeviceReset(GraphicsProvider *pProvider);

private:
  static D3DDECLUSAGE GetVertexUsageType(VertexUsageType::Enum type);
  static D3DDECLTYPE GetVertexType(VertexDataType::Enum type);
  static UINT CalcOffset(VertexDataType::Enum type);

  static const D3DVERTEXELEMENT9 kEndElement;
  std::vector<VertexElement> m_elements;
  GraphicsProvider *m_pProvider;
  IDirect3DVertexDeclaration9 *m_pDecl;
};

inline void VertexDeclaration::AddElement(const VertexElement &elem)
{
  m_elements.push_back(elem);
}

inline void VertexDeclaration::AddElement(VertexDataType::Enum vdt,
  VertexUsageType::Enum vut, UINT slot)
{
  AddElement(VertexElement(vdt, vut, slot));
}

inline void VertexDeclaration::RemoveElement(UINT index)
{
  auto it = m_elements.begin();
  m_elements.erase((it + index));
}

} // namespace shinybear

#endif // SHINYBEAR_VERTEXDECLARATION_H