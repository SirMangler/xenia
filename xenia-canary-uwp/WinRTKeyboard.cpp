#include "WinRTKeyboard.h"

#include <winrt/Windows.UI.ViewManagement.Core.h>
#pragma optimize("", off)
namespace UWP {
std::vector<char>* m_text_input;

bool IsKeyboardShowing() {
  //winrt::Windows::UI::ViewManagement::Core::CoreInputView::GetForCurrentView()
  //    .PrimaryViewShowing();
}

void ShowKeyboard(std::vector<char>* input_text) {
    //winrt::Windows::UI::ViewManagement::Core::CoreInputView::GetForCurrentView()
    //    .TryShow(winrt::Windows::UI::ViewManagement::Core::CoreInputViewKind::
    //                    Keyboard);

    winrt::Windows::UI::ViewManagement::Core::CoreInputView::GetForCurrentView()
      .TryShowPrimaryView();
    m_text_input = input_text;
}

void HandleCharacter(uint32_t keycode) {
    size_t str_len = strlen(m_text_input->data());
    if (keycode == 0x08 /* backspace */) {
      if (str_len > 0)
        m_text_input->data()[str_len - 1] = '\0';
    } else {
      if (str_len < 32)
        m_text_input->data()[str_len] = static_cast<char>(keycode);
    }
}
}  // namespace UWP
#pragma optimize("", on)