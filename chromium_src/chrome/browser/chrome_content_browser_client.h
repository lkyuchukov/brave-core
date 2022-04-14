/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_CHROME_BROWSER_CHROME_CONTENT_BROWSER_CLIENT_H_
#define BRAVE_CHROMIUM_SRC_CHROME_BROWSER_CHROME_CONTENT_BROWSER_CLIENT_H_

#define CHROME_CONTENT_BROWSER_CLIENT_H \
 private:                               \
  friend class BraveContentBrowserClient;

#include "src/chrome/browser/chrome_content_browser_client.h"

#undef CHROME_CONTENT_BROWSER_CLIENT_H

#endif  // BRAVE_CHROMIUM_SRC_CHROME_BROWSER_CHROME_CONTENT_BROWSER_CLIENT_H_
