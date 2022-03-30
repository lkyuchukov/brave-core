/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define PopulateFrameBinders PopulateFrameBinders_ChromiumImpl
#include "src/content/browser/browser_interface_binders.cc"
#undef PopulateFrameBinders

namespace content {
namespace internal {

void PopulateFrameBinders(RenderFrameHostImpl* host, mojo::BinderMap* map) {
  PopulateFrameBinders_ChromiumImpl(host, map);

  // Keep handwriting recognition disabled
  map->Add<handwriting::mojom::HandwritingRecognitionService>(
      base::DoNothing());
}

}  // namespace internal
}  // namespace content
