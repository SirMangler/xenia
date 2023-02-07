/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_UI_WINDOWED_APP_CONTEXT_UWP
#define XENIA_UI_WINDOWED_APP_CONTEXT_UWP

#include "xenia/ui/windowed_app_context.h"

namespace xe {
namespace ui {

class UWPWindowedAppContext final : public WindowedAppContext {
 public:
  explicit UWPWindowedAppContext();
  ~UWPWindowedAppContext();
  void PlatformQuitFromUIThread();
  void NotifyUILoopOfPendingFunctions();
};

}  // namespace ui
}  // namespace xe

#endif  // XENIA_UI_WINDOWED_APP_CONTEXT_UWP
