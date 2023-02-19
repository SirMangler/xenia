#define NOMINMAX
#include <windows.h>

#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>

#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/windows.graphics.display.core.h>

#include <Gamingdeviceinformation.h>

#include "XeniaUWP.h"
#include "UWPUtil.h"
#include "WinRTKeyboard.h"

    //#include "xenia/base/main_win.h"

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
    CompositionTarget m_target{ nullptr };
    VisualCollection m_visuals{ nullptr };
    Visual m_selected{ nullptr };
    float2 m_offset{};
    hstring m_launchOnExit;

    IFrameworkView CreateView()
    {
        return *this;
    }

    void Initialize(CoreApplicationView const& v)
    {
      v.Activated({this, &App::OnActivate});
    }

    void Load(hstring const&)
    {
    }

    void Uninitialize()
    {
    }

    void Run()
    {
        auto navigation = winrt::Windows::UI::Core::SystemNavigationManager::GetForCurrentView();

        // UWP on Xbox One triggers a back request whenever the B button is
        // pressed which can result in the app being suspended if unhandled
        navigation.BackRequested(
            [](const winrt::Windows::Foundation::IInspectable&,
               const BackRequestedEventArgs& args) { args.Handled(true); });

        winrt::Windows::Graphics::Display::DisplayInformation displayInfo =
            winrt::Windows::Graphics::Display::DisplayInformation::
                GetForCurrentView();
        if (displayInfo) {
          UWP::SetDPI(displayInfo.LogicalDpi());
        }

        UWP::StartXenia();

        while (true) {
          CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(
              CoreProcessEventsOption::ProcessAllIfPresent);

          UWP::ExecutePendingFunctionsFromUIThread();

          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        CoreDispatcher dispatcher =
            CoreWindow::GetForCurrentThread().Dispatcher();
        dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);

        if (!m_launchOnExit.empty()) {
          winrt::Windows::Foundation::Uri m_uri{m_launchOnExit};
          auto asyncOperation =
              winrt::Windows::System::Launcher::LaunchUriAsync(m_uri);
          asyncOperation.Completed(
              [](winrt::Windows::Foundation::IAsyncOperation<bool> const&
                     sender,
                 winrt::Windows::Foundation::AsyncStatus const asyncStatus) {
                CoreApplication::Exit();
                return;
              });
        }
    }

    void OnActivate(
        const winrt::Windows::ApplicationModel::Core::CoreApplicationView& v,
        const winrt::Windows::ApplicationModel::Activation::IActivatedEventArgs&
            args) {
        std::stringstream filePath;

        if (args.Kind() ==
            Windows::ApplicationModel::Activation::ActivationKind::Protocol) {
          auto protocolActivatedEventArgs{
              args.as<Windows::ApplicationModel::Activation::
                          ProtocolActivatedEventArgs>()};
          auto query = protocolActivatedEventArgs.Uri().QueryParsed();

          for (uint32_t i = 0; i < query.Size(); i++) {
            auto arg = query.GetAt(i);

            // parse command line string
            if (arg.Name() == winrt::hstring(L"cmd")) {
              std::string argVal = winrt::to_string(arg.Value());

              // Strip the executable from the cmd argument
              if (argVal.rfind("xeniacanary.exe", 0) == 0) {
                argVal = argVal.substr(15, argVal.length());
              }

              std::istringstream iss(argVal);
              std::string s;

              // Maintain slashes while reading the quotes
              while (iss >> std::quoted(s, '"', (char)0)) {
                filePath << s;
              }
            } else if (arg.Name() == winrt::hstring(L"launchOnExit")) {
              // For if we want to return to a frontend
              m_launchOnExit = arg.Value();
            }
          }
        }

        std::string gamePath = filePath.str();
        if (!gamePath.empty() && gamePath != "") {
          UWP::SetAutomaticLaunch(gamePath);
        }

        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();
    }

    void OnCharacterReceived(
        winrt::Windows::UI::Core::CoreWindow const& /* sender */,
        winrt::Windows::UI::Core::
            CharacterReceivedEventArgs const& e /* args */) {
        UWP::HandleCharacter(e.KeyCode());
    }

    void SetWindow(CoreWindow const & window)
    {
        window.CharacterReceived({this, &App::OnCharacterReceived});
    }
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    winrt::init_apartment();
    CoreApplication::Run(make<App>());
    winrt::uninit_apartment();
}
