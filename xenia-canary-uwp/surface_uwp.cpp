#include "surface_uwp.h"

#include <windows.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/windows.graphics.display.core.h>
#include <Gamingdeviceinformation.h>

xe::ui::UWPWindowSurface::UWPWindowSurface() { }

bool xe::ui::UWPWindowSurface::GetSizeImpl(uint32_t& width_out,
                                           uint32_t& height_out) const {
  width_out = 1920;
  height_out = 1080;
  // Temporarily disabled until the scaling/zoom issue is resolved
  //GAMING_DEVICE_MODEL_INFORMATION info = {};
  //GetGamingDeviceModelInformation(&info);
  //if (info.vendorId == GAMING_DEVICE_VENDOR_ID_MICROSOFT) {
  //  auto hdi = winrt::Windows::Graphics::Display::Core::HdmiDisplayInformation::
  //      GetForCurrentView();
  //  if (hdi) {
  //    width_out = hdi.GetCurrentDisplayMode().ResolutionWidthInRawPixels();
  //    height_out = hdi.GetCurrentDisplayMode().ResolutionHeightInRawPixels();
  //  }
  //}

  return true;
}