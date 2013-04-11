#ifndef DXGRAPHICSPROVIDER_H
#define DXGRAPHICSPROVIDER_H

#include "graphics\IGraphicsProvider.h"
#include "graphics\DisplayMode.h"
#include <d3d9.h>
#include <Windows.h>


// Forward declarations


namespace shinybear {

class DXGraphicsProvider : public IGraphicsProvider {
public:
  DXGraphicsProvider(HWND hTargetWindow);
  ~DXGraphicsProvider();
  // Override from IGraphicsProvider
  bool Initialize();

  // Override from IGraphicsProvider
  bool CheckHardware();

  bool IsFullscreen();
  bool ToggleFullscreen(bool value);

  const DisplayMode& GetDisplayMode();
  bool SetDisplayMode(const DisplayMode &mode);

  const MultiSampleMode& GetMultiSampleMode();
  bool SetMultiSampleMode(const MultiSampleMode &mode);

  bool IsDeviceLost();
  bool ResetDevice();

  void ApplyChanges();

  // TODO REMOVE
  IDirect3DDevice9 *GetDevice() { return m_pDevice; }

  DisplayMode* GetValidDisplayModes(UINT *numModes);
  MultiSampleMode* GetValidMultiSampleModes(UINT *numModes);
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

  // Returns all possible MSAA modes.
  MultiSampleMode* GetAllMSAAModes(UINT *numModes);
  
  // Filters out invalid MSAA modes from the input array,
  // and returns a new array with valid modes.
  MultiSampleMode* FilterInvalidMSAAModes(MultiSampleMode *pModes, UINT count, UINT *numValid);

  bool IsMSAAValid(const MultiSampleMode &mode);

  // Helper function to convert from integer sample input to a DirectX type.
  D3DMULTISAMPLE_TYPE GetMultiSampleType(const MultiSampleMode &mode);


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
inline bool DXGraphicsProvider::IsFullscreen() {
  return m_currentDisplayMode.fullscreen;
}
  

} // namespace shinybear


#endif