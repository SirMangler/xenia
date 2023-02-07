#define NOMINMAX
#include <windows.h>

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

#include <Gamingdeviceinformation.h>

#include <XeniaUWP.h>

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

    IFrameworkView CreateView()
    {
        return *this;
    }

    void Initialize(CoreApplicationView const &)
    {
    }

    void Load(hstring const&)
    {
    }

    void Uninitialize()
    {
    }

    void Run()
    {
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();

        auto navigation = winrt::Windows::UI::Core::SystemNavigationManager::GetForCurrentView();

        // UWP on Xbox One triggers a back request whenever the B button is
        // pressed which can result in the app being suspended if unhandled
        navigation.BackRequested(
            [](const winrt::Windows::Foundation::IInspectable&,
               const BackRequestedEventArgs& args) { args.Handled(true); });

        UWP::StartXenia();

        while (true) {
          CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(
              CoreProcessEventsOption::ProcessAllIfPresent);

          UWP::ExecutePendingFunctionsFromUIThread();

          std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        CoreDispatcher dispatcher = window.Dispatcher();
        dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
    }

    void SetWindow(CoreWindow const & window)
    {

    }
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(make<App>());
}
