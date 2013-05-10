#ifndef DXGRAPHICSPROVIDER_H
#define DXGRAPHICSPROVIDER_H
#include "base\system\DisplayMode.h"
#include "util\Color4f.h"

#include <d3d9.h>
#include <Windows.h>
#include <vector>

namespace shinybear { class IGraphicsResource; }

namespace shinybear
{

class GraphicsProvider
{
public:
  GraphicsProvider(HWND hTargetWindow);
  ~GraphicsProvider();

  // Called to first initialize the game in Run().
  bool Initialize();

  // Fullscreen mode.
  bool IsFullscreen();
  bool ToggleFullscreen(bool value);

  // Returns current display mode in use.
  const DisplayMode& GetDisplayMode();

  // Sets the current displaymode, effective after ApplyChanges() has been called.
  bool SetDisplayMode(const DisplayMode &mode);

  // Retrieve current multisample mode.
  const MultiSampleMode& GetMultiSampleMode();

  // Sets the current multisample mode, effective after ApplyChanges() has been called.
  bool SetMultiSampleMode(const MultiSampleMode &mode);

  // returns the state of the graphics device.
  HRESULT GetDeviceState();

  // Performs a device-reset;
  HRESULT ResetDevice();

  // Re-creates the device with new settings if any.
  void ApplyChanges();

  IDirect3DDevice9 *GetDevice() const;

  DisplayMode* GetValidDisplayModes(UINT *numModes);
  MultiSampleMode* GetValidMultiSampleModes(UINT *numModes);

  bool CheckDisplayMode(const DisplayMode &mode);

  // Adds a resource to the notification list.
  void AddResource(IGraphicsResource *pResource);

  // Removes a resource from the notification list.
  void RemoveResource(IGraphicsResource *pResource);

  // Rendering Functions
#pragma region RenderingFunctions
  
  void Clear(const Color4f &color);
  void Present();

#pragma endregion 
private:
  // Default format.
  static const D3DFORMAT kDefaultFormat;
 
  // Returns an allocated array containing all the displaymodes
  // the device supports with the current displayformat.
  DisplayMode* GetAllDisplayModes(UINT *numModes);

  // Takes in an array of displaymodes and checks
  // using IDirect9::CheckDeviceType, returns an allocated array
  // containing the ones that made the check.
  DisplayMode* FilterInvalidDisplayModes(DisplayMode *pModes, UINT modeCount, UINT *numValid);

  // Helper function to pretty up code.
  bool IsDisplayModeValid(const DisplayMode &mode);

  bool IsMSAAValid(const MultiSampleMode &mode);

  // Returns all possible MSAA modes.
  MultiSampleMode* GetAllMSAAModes(UINT *numModes);
  
  // Filters out invalid MSAA modes from the input array,
  // and returns a new array with valid modes.
  MultiSampleMode* FilterInvalidMSAAModes(MultiSampleMode *pModes, UINT count, UINT *numValid);

  // Helper function to convert from integer sample input to a DirectX type.
  D3DMULTISAMPLE_TYPE GetMultiSampleType(const MultiSampleMode &mode);

  UINT m_numValidDisplayModes;
  DisplayMode *m_pValidDisplayModes;
  UINT m_adapterIndex;
  HWND m_hTargetWindow;
  IDirect3D9 *m_pD3DCreate;
  IDirect3DDevice9 *m_pDevice;
  DisplayMode m_currentDisplayMode;
  DisplayMode m_nextDisplayMode;
  MultiSampleMode m_currentMSAAMode;
  MultiSampleMode m_nextMSAAMode;
  D3DPRESENT_PARAMETERS m_d3dPresent;
};


// Inlines
inline IDirect3DDevice9 * GraphicsProvider::GetDevice() const
{
  return m_pDevice;
}

inline bool GraphicsProvider::IsFullscreen()
{
  return m_currentDisplayMode.fullscreen;
}

inline void GraphicsProvider::Clear(const Color4f &color)
{
  m_pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL | D3DCLEAR_TARGET,
    color, 1.0f, 0);
}

inline void GraphicsProvider::Present()
{
  m_pDevice->Present(0, 0, 0, 0);
}
  

} // namespace shinybear


#endif