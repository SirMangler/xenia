#include "UWPUtil.h"

#include "xenia/emulator.h"

#include <iostream>
#include <fstream> 
#include <string>

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

  auto folder = co_await openPicker.PickSingleFolderAsync();
  std::string path = "";
  if (folder) {
    path = winrt::to_string(folder.Path().data());
  }

  callback(path);
}

void OpenGamePicker(xe::Emulator* emu) { PickGame(emu); }

void SelectFolder(std::function<void(std::string)> callback) {
  PickFolderAsync(callback);
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
int GetCoreDPI() {
  winrt::Windows::Graphics::Display::DisplayInformation displayInfo = winrt::
      Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
  if (displayInfo) {
    return displayInfo.LogicalDpi();
  }

  return 96;
}
}