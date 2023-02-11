#include "window_uwp.h"
#include "surface_uwp.h"

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

#include <XeniaUWP.h>
#include <UWPUtil.h>

namespace xe {
namespace ui {
UWPWindow::UWPWindow(WindowedAppContext& app_context,
                     const std::string_view title, uint32_t width,
                     uint32_t height)
    : Window(app_context, title, width, height) {
  UWP::RegisterXeniaWindow(this);
}

UWPWindow::~UWPWindow() {}

void UWPWindow::SetXInputDriver(xe::hid::xinput::XInputInputDriver* driver) {
  input_driver = driver;
}

// May return nullptr!
xe::hid::xinput::XInputInputDriver* UWPWindow::xinputdriver() {
  return input_driver;
}

void UWPWindow::ClearXInputDriver() { input_driver = nullptr; }

std::unique_ptr<Window> Window::Create(WindowedAppContext& app_context,
                                       const std::string_view title,
                                       uint32_t desired_logical_width,
                                       uint32_t desired_logical_height) {
  return std::make_unique<UWPWindow>(
      app_context, title, desired_logical_width, desired_logical_height);
}

uint32_t UWPWindow::GetLatestDpiImpl() const { return UWP::GetCoreDPI(); }

std::unique_ptr<Surface> UWPWindow::CreateSurfaceImpl(
    Surface::TypeFlags allowed_types) {
  return std::make_unique<UWPWindowSurface>();
}

bool UWPWindow::OpenImpl() {
  uint32_t width = 1920;
  uint32_t height = 1080;
  // Temporarily disabled until the scaling/zoom issue is resolved
  //GAMING_DEVICE_MODEL_INFORMATION info = {};
  //GetGamingDeviceModelInformation(&info);
  //if (info.vendorId == GAMING_DEVICE_VENDOR_ID_MICROSOFT) {
  //  auto hdi = winrt::Windows::Graphics::Display::Core::HdmiDisplayInformation::
  //      GetForCurrentView();
  //  if (hdi) {
  //    width = hdi.GetCurrentDisplayMode().ResolutionWidthInRawPixels();
  //    height = hdi.GetCurrentDisplayMode().ResolutionHeightInRawPixels();
  //  }
  //}

  WindowDestructionReceiver destruction_receiver(this);
  OnActualSizeUpdate(width, height,
                     destruction_receiver);  
  return true; 
}

void UWPWindow::RequestPaintImpl() { 
  UWP::UpdateImGuiIO();
  OnPaint(); 
}

void UWPWindow::RequestCloseImpl() {}

void UWPWindow::FocusImpl() {}

}  // namespace ui
}  // namespace xe