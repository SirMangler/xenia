#pragma once

#include <functional>
#include <string>

namespace xe {
class Emulator;
}

namespace UWP
{
void SelectGameFromWinRT(xe::Emulator* emu);
bool HasGamePath();
void SelectFolder(std::function<void(std::string)> callback);
std::string GetLocalCache();
std::string GetLocalState();
int GetCoreDPI();
void SetAutomaticLaunch(std::string game_path);
void SetDPI(int DPI);
}