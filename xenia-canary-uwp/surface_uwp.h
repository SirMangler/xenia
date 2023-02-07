/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_UI_SURFACE_UWP
#define XENIA_UI_SURFACE_UWP
#include "xenia/ui/surface.h"

namespace xe {
namespace ui {

class UWPWindowSurface final : public Surface {
 public:
  UWPWindowSurface();
  TypeIndex GetType() const override { return kTypeIndex_UWPCore; }

 protected:
  bool GetSizeImpl(uint32_t& width_out, uint32_t& height_out) const override;
};

}  // namespace ui
}  // namespace xe

#endif  // XENIA_UI_SURFACE_UWP