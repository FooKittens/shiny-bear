#ifndef SHINYBEAR_VERTEXMANAGER_H
#define SHINYBEAR_VERTEXMANAGER_H

#include "resource\IResource.h"
#include "graphics\VertexFormats.h"
#include "graphics\VertexDeclaration.h"

#include <map>

namespace shinybear { class GraphicsProvider; class ResourceManager; }

namespace shinybear
{

// Inherits from IResource to get cleaned up when application exits.
class VertexManager
{
public:
  friend class ResourceManager;

  // Initializes and creates all vertex declarations.
  static void Initialize(GraphicsProvider *pProvider);

  static VertexDeclaration * const GetVertexDeclaration(VertexDecl::E type);
  static void RegisterVertexDeclaration(VertexDecl::E type, VertexDeclaration *pDecl);

  ResourceType::Enum GetType() const { return ResourceType::RT_MISC; }

  static void Cleanup();
private:
  typedef std::pair<VertexDecl::E, VertexDeclaration*> VertexMapping;

  static VertexManager *pSingleton;
  static bool isInitialized;

  VertexManager(GraphicsProvider *pProvider);
  ~VertexManager();

  // Forwarded to from static method.
  VertexDeclaration * const SafeGetDecl(VertexDecl::E type);
  void SafeRegDecl(VertexDecl::E type, VertexDeclaration *pDecl);

  // Creates all built in vertex declarations.
  void CreateDefaultDecl();

  bool DeclExists(VertexDecl::E type);

  GraphicsProvider *m_pProvider;
  std::map<VertexDecl::E, VertexDeclaration*> m_declCollection;
};

} // namespace shinybear

#endif // SHINYBEAR_VERTEX_MANAGER_H