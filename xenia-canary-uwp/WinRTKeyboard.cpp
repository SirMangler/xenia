#include "WinRTKeyboard.h"

#include <winrt/Windows.UI.ViewManagement.Core.h>

namespace UWP {
std::vector<uint32_t> g_char_buffer;
std::mutex g_buffer_mutex;

void ShowKeyboard() {
  winrt::Windows::UI::ViewManagement::Core::CoreInputView::GetForCurrentView()
      .TryShowPrimaryView();
}

void HandleCharacter(uint32_t keycode) {
  std::unique_lock lk(g_buffer_mutex);
  g_char_buffer.push_back(keycode);
}
}  // namespace UWP