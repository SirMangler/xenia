#include "xenia/ui/window.h"
#include "xenia/hid/xinput/xinput_input_driver.h"

namespace xe {
namespace ui {

class UWPWindow : public Window {
 public:
  UWPWindow(WindowedAppContext& app_context, const std::string_view title,
            uint32_t width, uint32_t height);
  ~UWPWindow() override;

  uint32_t GetMediumDpi() const override { return 96; }
  void SetXInputDriver(xe::hid::xinput::XInputInputDriver* driver);
  xe::hid::xinput::XInputInputDriver* xinputdriver();
  void ClearXInputDriver();

protected:
    uint32_t GetLatestDpiImpl() const override;

    std::unique_ptr<Surface> CreateSurfaceImpl(
        Surface::TypeFlags allowed_types) override;

    bool OpenImpl() override;
    void RequestCloseImpl() override;
    // Mouse capture seems to happen implicitly compared to Windows.
    void FocusImpl() override;
    void RequestPaintImpl() override;

    xe::hid::xinput::XInputInputDriver* input_driver = nullptr;
};
}  // namespace ui
}  // namespace xe