#pragma once

#include <string>
#include <vector>

namespace UWP {
bool IsKeyboardShowing();
void ShowKeyboard(std::vector<char>* input_text);
void HandleCharacter(uint32_t keycode);
}