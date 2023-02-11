#pragma once

#include <functional>
#include <string>

namespace xe {
class Emulator;
}

namespace UWP
{
void OpenGamePicker(xe::Emulator* emu);
void SelectFolder(std::function<void(std::string)> callback);
std::string GetLocalCache();
std::string GetLocalState();
int GetCoreDPI();
}