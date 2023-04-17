#include "UWPUtil.h"

#include "xenia/emulator.h"

#include <iostream>
#include <fstream> 
#include <string>
#include <ppl.h>
#include <ppltasks.h>
#include <agents.h>

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

using namespace winrt::Windows::Storage::Pickers;
namespace UWP {
std::string m_game_path = "";
int m_DPI = 96;
bool m_ui_open = false;

winrt::fire_and_forget PickGame(xe::Emulator* emu) {
  FileOpenPicker openPicker;
  openPicker.ViewMode(PickerViewMode::List);
  openPicker.SuggestedStartLocation(PickerLocationId::ComputerFolder);
  openPicker.FileTypeFilter().Append(L"*");

  auto file = co_await openPicker.PickSingleFileAsync();
  if (file) {
    std::string path = winrt::to_string(file.Path().data());

    emu->LaunchPath(path);
  }
}

winrt::fire_and_forget PickFolderAsync(
    std::function<void(std::string)> callback) {
  FolderPicker openPicker;
  openPicker.ViewMode(PickerViewMode::List);
  openPicker.SuggestedStartLocation(PickerLocationId::ComputerFolder);
  openPicker.FileTypeFilter().Append(L"*");

  auto folder = co_await openPicker.PickSingleFolderAsync();
  std::string path = "";
  if (folder) {
    path = winrt::to_string(folder.Path().data());
  }

  callback(path);
}

winrt::fire_and_forget PickFileAsync(
    std::function<void(std::string)> callback) {
  FileOpenPicker openPicker;
  openPicker.ViewMode(PickerViewMode::List);
  openPicker.SuggestedStartLocation(PickerLocationId::ComputerFolder);
  openPicker.FileTypeFilter().Append(L"*");

  auto folder = co_await openPicker.PickSingleFileAsync();
  std::string path = "";
  if (folder) {
    path = winrt::to_string(folder.Path().data());
  }

  callback(path);
}

bool HasGamePath() { return m_game_path != ""; }

void SelectGameFromWinRT(xe::Emulator* emu) { 
  if (m_game_path == "")
    PickGame(emu);
  else
    emu->LaunchPath(m_game_path);
}

void SelectFolder(std::function<void(std::string)> callback) {
  PickFolderAsync(callback);
}

void SelectFile(std::function<void(std::string)> callback) {
  PickFileAsync(callback);
}

std::string GetLocalCache() {
  return winrt::to_string(winrt::Windows::Storage::ApplicationData::Current()
                              .LocalCacheFolder()
                              .Path());
}

std::string GetLocalState() {
  return winrt::to_string(
      winrt::Windows::Storage::ApplicationData::Current().LocalFolder().Path());
}

int GetCoreDPI() { return m_DPI; }

void SetAutomaticLaunch(std::string game_path) { m_game_path = game_path; }
void SetDPI(int DPI) { m_DPI = DPI; }
bool IsUIOpen() { return m_ui_open; }
void SetUIOpen(bool is_open) { m_ui_open = is_open; }
}