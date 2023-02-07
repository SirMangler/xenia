/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "windowed_app_context_uwp.h"

namespace xe {
namespace ui {
UWPWindowedAppContext::UWPWindowedAppContext() {}
UWPWindowedAppContext::~UWPWindowedAppContext() {}
void UWPWindowedAppContext::PlatformQuitFromUIThread() {}
void UWPWindowedAppContext::NotifyUILoopOfPendingFunctions() {}
}  // namespace ui
}  // namespace xe
