/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_COMPONENT_LOADER_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_COMPONENT_LOADER_H_

#define BRAVE_COMPONENT_LOADER_H \
 private:                        \
  friend class BraveComponentLoader;

#define AddDefaultComponentExtensions virtual AddDefaultComponentExtensions
#define AddHangoutServicesExtension virtual AddHangoutServicesExtension

#include "src/chrome/browser/extensions/component_loader.h"

#undef AddHangoutServicesExtension
#undef AddDefaultComponentExtensions
#undef BRAVE_COMPONENT_LOADER_H

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_COMPONENT_LOADER_H_
