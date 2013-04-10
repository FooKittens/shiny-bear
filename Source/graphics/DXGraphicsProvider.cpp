#include "graphics\DXGraphicsProvider.h"
#include "util\SBUtil.h"
#include <d3d9.h>
#include <Windows.h>
#include <cassert>

namespace shinybear {

// Constant default format used by DirectX
const D3DFORMAT DXGraphicsProvider::kDefaultFormat = D3DFMT_X8R8G8B8;

DXGraphicsProvider::DXGraphicsProvider(HWND hTargetWindow) {
  m_pD3DCreate = nullptr;
  m_hTargetWindow = hTargetWindow;
  m_isFullscreen = false;
  m_pDevice = nullptr;

  // Default to the default adapter.
  m_adapterIndex = D3DADAPTER_DEFAULT;
}

bool DXGraphicsProvider::Initialize() {
  m_pD3DCreate = Direct3DCreate9(D3D_SDK_VERSION);
  
  assert(m_pD3DCreate && "Failed to create Direct3D9 object!");

  return true;
}

bool DXGraphicsProvider::CheckHardware() {

  return true;
}

bool DXGraphicsProvider::IsFullScreen() {
  return m_isFullscreen;
}

bool DXGraphicsProvider::ToggleFullscreen(bool value) {
  // Attempted to set the current mode again?
  if(value == m_isFullscreen) {
    return false;
  }
  
  m_nextDisplayMode.fullscreen = value;
  return true;
}

bool DXGraphicsProvider::IsDeviceLost() {
  return false;
}

bool DXGraphicsProvider::ResetDevice() {
  return true;
}

const DisplayMode& DXGraphicsProvider::GetDisplayMode() {
  return m_currentDisplayMode;
}

const MultiSampleMode& DXGraphicsProvider::GetMultiSampleMode() {
  return m_currentMSAAMode;
}

// Returns an array of all the valid 
DisplayMode *DXGraphicsProvider::GetValidDisplayModes(UINT *numModes) {
  assert(m_pD3DCreate && "GraphicsProvider must be"
    " initialized before getting displaymode!");

  // Retrieve all display modes.
  UINT modeCount;
  DisplayMode *pModes = GetAllDisplayModes(&modeCount);

  // Get a new array containing only the valid ones.
  UINT validCount;
  DisplayMode *pValidatedModes =
    FilterInvalidDisplayModes(pModes, modeCount, &validCount);
  
  // Delete the invalid modes.
  delete[] pModes;

  // Set the output parameter numModes.
  *numModes = validCount;

  return pValidatedModes;
}

// Returns all the displaymodes the adapter supports for 
DisplayMode *DXGraphicsProvider::GetAllDisplayModes(UINT *pNumModes) {
  // Get mode count
  UINT modeCount = m_pD3DCreate->GetAdapterModeCount(m_adapterIndex, kDefaultFormat);
  
  if(modeCount == 0) {
    *pNumModes = 0;
    return nullptr;
  }

  DisplayMode *pModes = new DisplayMode[modeCount];

  // Initialize a list of displaymodes for the default adapter.
  D3DDISPLAYMODE d3dMode;
  for(UINT i = 0; i < modeCount; ++i) {
    DisplayMode mode = pModes[i];
    HR(m_pD3DCreate->EnumAdapterModes(m_adapterIndex, kDefaultFormat, i, &d3dMode));
    mode.width = d3dMode.Width;
    mode.height = d3dMode.Height;
    mode.refreshRate = d3dMode.RefreshRate;
  }

  *pNumModes = modeCount;
  return pModes;
}

// Filters out all invalid displaymodes from the input array
// Returns an array with the valid modes, and the size of that array in its
// output parameter.
DisplayMode* DXGraphicsProvider::FilterInvalidDisplayModes(DisplayMode *pModes,
  UINT modeCount, UINT *numValid) {

  // This list will hold the state(valid/invalid) of each displaymode.
  bool *pIndexStates = new bool[modeCount];
  int validCount = 0;

  // Checks each display mode and counts how many are valid.
  for(UINT i = 0; i < modeCount; ++i) {
    // Default to invalid.
    pIndexStates[i] = false;

    if(IsDisplayModeValid(pModes[i])) {
      pIndexStates[i] = true;
      ++validCount;
    }
  }

  // Will hold the valid displaymodes.
  DisplayMode *pValidModes = new DisplayMode[validCount];
  int validIndex = 0;

  // Loop through all modes and set the valid modes.
  for(UINT i = 0; i < modeCount; ++i) {
    if(pIndexStates[i]) {
      pValidModes[validIndex++] = pModes[i];
    }
  }
  
  // Free the state array.
  delete[] pIndexStates;

  // Set output parameter.
  *numValid = (UINT)validCount;

  return pValidModes;
}

// Helper function for determining if a displaymode is valid.
bool DXGraphicsProvider::IsDisplayModeValid(const DisplayMode &mode) {

  assert(m_pD3DCreate && "No D3DCreate object!");

  HRESULT hr = m_pD3DCreate->CheckDeviceType(m_adapterIndex, D3DDEVTYPE_HAL,
      kDefaultFormat, kDefaultFormat, !mode.fullscreen);

  // See if format failed.
  if(FAILED(hr)) {
    return false;
  }

  return SUCCEEDED(hr);
}

bool DXGraphicsProvider::SetDisplayMode(const DisplayMode &mode) {
  
  assert(IsDisplayModeValid(mode) && "Displaymode is not valid!");

  m_nextDisplayMode = mode;

  return true;
}

MultiSampleMode *DXGraphicsProvider::GetValidMultiSampleModes(UINT *numModes) {

  UINT modeCount;
  MultiSampleMode *pModes = GetAllMSAAModes(&modeCount);

  UINT validCount;
  MultiSampleMode *pValidated = FilterInvalidMSAAModes(
    pModes, modeCount, &validCount);

  // Free first array.
  delete[] pModes;

  // Set output parameter.
  *numModes = validCount;

  return pValidated;
}

// Returns all MSAA modes, from [0, 16] quality and [0, 16] samples.
MultiSampleMode *DXGraphicsProvider::GetAllMSAAModes(UINT *numModes) {
  const UINT kMaxSamples = 16;
  const UINT kMaxQuality = 16;

  // There are 16 quality levels and 16 sample counts.
  const UINT kMaxModes = (kMaxSamples + 1) * (kMaxQuality + 1);

  // Array with enough space to hold all the different combinations.
  MultiSampleMode *pModes = new MultiSampleMode[kMaxModes];

  // Setup all MSAA modes.
  for(UINT quality = 0; quality <= kMaxQuality; ++quality) {
    for(UINT samples = 0; samples <= kMaxSamples; ++samples) {
      MultiSampleMode *pMode = &pModes[quality * kMaxSamples + samples];

      pMode->quality = quality;
      pMode->samples = samples;
    }
  }

  *numModes = kMaxModes;
  return pModes;
}

// Returns an array containing the valid MSAA modes from input array.
// Stores the size of the new array in numValid.
MultiSampleMode *DXGraphicsProvider::FilterInvalidMSAAModes(
  MultiSampleMode *pModes, UINT count, UINT *numValid) {

  // Store results for wether an array index is valid or not.
  bool *pIndexStates = new bool[count];
  UINT validCount = 0;
  for(UINT i = 0; i < count; ++i) {
    pIndexStates[i] = false;

    if(IsMSAAValid(pModes[i])) {
      pIndexStates[i] = true;
      ++validCount;
    }
  }

  
  // Store the valid modes in a new array.
  MultiSampleMode *pValid = new MultiSampleMode[validCount];
  UINT vi = 0;
  for(UINT i = 0; i < count; ++i) {
    if(pIndexStates[i]) {
      pValid[vi++] = pModes[i];
    }
  }

  // Set output parameter.
  *numValid = validCount;

  return pValid;
}

// Helper for retrieving the correct DirectX multisample type from
// a sample count.
D3DMULTISAMPLE_TYPE DXGraphicsProvider::GetMultiSampleType(
  const MultiSampleMode &mode) {

  if(mode.samples > 16) {
    return D3DMULTISAMPLE_16_SAMPLES;
  }

  switch(mode.samples) {
    case 2:
      return D3DMULTISAMPLE_2_SAMPLES;
    case 3:
      return D3DMULTISAMPLE_3_SAMPLES;
    case 4:
      return D3DMULTISAMPLE_4_SAMPLES;
    case 5:
      return D3DMULTISAMPLE_5_SAMPLES;
    case 6:
      return D3DMULTISAMPLE_6_SAMPLES;
    case 7:
      return D3DMULTISAMPLE_7_SAMPLES;
    case 8:
      return D3DMULTISAMPLE_8_SAMPLES;
    case 9:
      return D3DMULTISAMPLE_9_SAMPLES;
    case 10:
      return D3DMULTISAMPLE_10_SAMPLES;
    case 11:
      return D3DMULTISAMPLE_11_SAMPLES;
    case 12:
      return D3DMULTISAMPLE_12_SAMPLES;
    case 13:
      return D3DMULTISAMPLE_13_SAMPLES;
    case 14:
      return D3DMULTISAMPLE_14_SAMPLES;
    case 15:
      return D3DMULTISAMPLE_15_SAMPLES;
    case 16:
      return D3DMULTISAMPLE_16_SAMPLES;
    default:
      return D3DMULTISAMPLE_NONE;
  }    
}

// Shorthand to test a if a multisample mode is valid.
bool DXGraphicsProvider::IsMSAAValid(const MultiSampleMode &mode) {
  // Type needed for DirectX.
  DWORD quality = mode.quality;

  HRESULT hr = m_pD3DCreate->CheckDeviceMultiSampleType(
    m_adapterIndex,
    D3DDEVTYPE_HAL,
    kDefaultFormat,
    !m_isFullscreen,
    GetMultiSampleType(mode),
    &quality
  );

  return SUCCEEDED(hr);
}

bool DXGraphicsProvider::SetMultiSampleMode(const MultiSampleMode &mode) {
  assert(IsMSAAValid(mode) && "Multisample type not valid!");

  m_nextMSAAMode = mode;
  return true;
}

void DXGraphicsProvider::ApplyChanges() {
  D3DCAPS9 deviceCaps;
  HR(m_pD3DCreate->GetDeviceCaps(m_adapterIndex, D3DDEVTYPE_HAL, &deviceCaps));

  D3DPRESENT_PARAMETERS dpp;
  dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
  dpp.BackBufferCount = 1;
  dpp.BackBufferFormat = kDefaultFormat;
  dpp.BackBufferHeight = m_nextDisplayMode.height;
  dpp.BackBufferWidth = m_nextDisplayMode.width;
  dpp.EnableAutoDepthStencil = false;
  dpp.Flags = 0;
  dpp.FullScreen_RefreshRateInHz = m_nextDisplayMode.refreshRate;
  dpp.hDeviceWindow = m_hTargetWindow;
  dpp.MultiSampleQuality = m_nextMSAAMode.quality;
  dpp.MultiSampleType = GetMultiSampleType(m_nextMSAAMode);
  dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  dpp.Windowed = !m_nextDisplayMode.fullscreen;

  // Check for hardware vertex processing.
  DWORD flags = 0;
  if(deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
  else
    flags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

  // Check if we can get a pure device.
  if(deviceCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
    flags |= D3DCREATE_PUREDEVICE;

  // Release the previous device.
  ReleaseCOM(m_pDevice);
  
  HR(m_pD3DCreate->CreateDevice(m_adapterIndex, D3DDEVTYPE_HAL,
    m_hTargetWindow, flags, &dpp, &m_pDevice));

  m_currentDisplayMode = m_nextDisplayMode;
  m_currentMSAAMode = m_nextMSAAMode;
}




} // namespace shinybear