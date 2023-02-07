#include "window_uwp.h"
#include "surface_uwp.h"
#include <XeniaUWP.h>

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

uint32_t UWPWindow::GetLatestDpiImpl() const { return 96; }

std::unique_ptr<Surface> UWPWindow::CreateSurfaceImpl(
    Surface::TypeFlags allowed_types) {
  return std::make_unique<UWPWindowSurface>();
}

bool UWPWindow::OpenImpl() { 
  WindowDestructionReceiver destruction_receiver(this);
  OnActualSizeUpdate(GetDesiredLogicalWidth(), GetDesiredLogicalHeight(),
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