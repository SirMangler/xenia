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
void SelectFile(std::function<void(std::string)> callback);
bool TestPathPermissions(std::string path);
std::string GetLocalCache();
std::string GetLocalState();
int GetCoreDPI();
void SetAutomaticLaunch(std::string game_path);
void SetDPI(int DPI);
bool IsUIOpen();
void SetUIOpen(bool is_open);
}