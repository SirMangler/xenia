#include "XeniaUWP.h"

#include "windowed_app_context_uwp.h"
#include "surface_uwp.h"
#include "window_uwp.h"

#include "xenia/emulator.h"
#include "xenia/base/filesystem.h"
#include "xenia/ui/windowed_app.h"
#include "xenia/base/cvar.h"
#include "xenia/base/logging.h"
#include "xenia/ui/window.h"
#include "xenia/ui/d3d12/d3d12_provider.h"
#include "xenia/gpu/d3d12/d3d12_graphics_system.h"
#include "xenia/hid/xinput/xinput_hid.h"
#include "xenia/hid/nop/nop_hid.h"
#include "xenia/apu/xaudio2/xaudio2_audio_system.h"
#include "xenia/config.h"
#include "xenia/base/main_win.h"
#include "third_party/imgui/imgui.h"
#include <UWPUtil.h>

using namespace xe;
using namespace xe::hid;

DECLARE_string(gamepaths);
DEFINE_string(gamepaths, "", "Paths the frontend will search for games.",
              "General");

static std::unique_ptr<ui::WindowedApp> app = nullptr;
static std::unique_ptr<ui::UWPWindowedAppContext> app_context = nullptr;
static ui::Window* s_window;
static Emulator* s_emulator;
static std::vector<std::string> s_paths;
static std::vector<std::string> s_games;

void UWP::StartXenia() {
  app_context = std::make_unique<ui::UWPWindowedAppContext>();
  app = xe::ui::GetWindowedAppCreator()(*app_context.get());

  xe::InitializeWin32App(app->GetName());

  if (app->OnInitialize()) {
    InitialisePaths();
    // to-do, remodel this so it doesn't instantly shutdown.
    //app->InvokeOnDestroy();
  }

  //xe::ShutdownWin32App();
}

void UWP::ExecutePendingFunctionsFromUIThread() {
  app_context->ExecutePendingFunctionsFromUIThread();

  if (s_window) {
    app_context->CallInUIThread([=]() { s_window->RequestPaint(); });
  }
}

void UWP::RegisterXeniaWindow(xe::ui::Window* window) { s_window = window; }

void UWP::UpdateImGuiIO() {
  auto driver = static_cast<xe::ui::UWPWindow*>(s_window)->xinputdriver();
  if (!driver) return;

  hid::X_INPUT_STATE state;
  if (driver->GetState(0, &state) != X_STATUS_SUCCESS) return;

  auto& io = ImGui::GetIO();
  io.AddKeyEvent(ImGuiKey_GamepadFaceDown, state.gamepad.buttons & X_INPUT_GAMEPAD_A);
  io.AddKeyEvent(ImGuiKey_GamepadFaceRight, state.gamepad.buttons & X_INPUT_GAMEPAD_B);
  io.AddKeyEvent(ImGuiKey_GamepadDpadLeft, state.gamepad.buttons & X_INPUT_GAMEPAD_DPAD_LEFT);
  io.AddKeyEvent(ImGuiKey_GamepadDpadRight, state.gamepad.buttons & X_INPUT_GAMEPAD_DPAD_RIGHT);
  io.AddKeyEvent(ImGuiKey_GamepadDpadUp, state.gamepad.buttons & X_INPUT_GAMEPAD_DPAD_UP);
  io.AddKeyEvent(ImGuiKey_GamepadDpadDown, state.gamepad.buttons & X_INPUT_GAMEPAD_DPAD_DOWN);
}

void RecurseFolderForGames(std::string path) {
  try {
    for (auto file : std::filesystem::directory_iterator(path)) {
      if (file.is_directory()) {
        RecurseFolderForGames(file.path().string());
        continue;
      }

      if (!file.is_regular_file()) continue;
      // only allow ISO/XEX or no extensions for now
      if (file.path().has_extension()) {
        if (file.path().extension().compare("xex") &&
            file.path().extension().compare("iso")) {
          continue;
        }
      }

      s_games.push_back(file.path().string());
    }
  } catch (std::exception) {
    // This folder can't be opened.
  }
}

void UWP::InitialisePaths() {
  RecurseFolderForGames(UWP::GetLocalCache());

  if (!cvars::gamepaths.empty()) {
    std::stringstream ss;
    std::string item;
    while (std::getline(ss, item, ';')) {
      if (item.empty()) continue;

      RecurseFolderForGames(item);
      s_paths.push_back(item);
    }
  }
}

std::vector<std::filesystem::path> UWP::GetGames() {
  std::vector<std::filesystem::path> paths;
  for (auto p : s_games) {
    paths.push_back(std::filesystem::path(p));
  }

  return paths;
}

void UWP::SetGamePaths(std::vector<std::string> paths) {
  s_paths.clear();
  s_paths.insert(s_paths.end(), paths.begin(), paths.end());
  std::stringstream ss;
  for (auto p : s_paths) {
    ss << p << ";";
  }

  cvars::gamepaths = ss.str();
  // todo, how save?
}

std::vector<std::string> UWP::GetPaths() { 
  return s_paths;
}