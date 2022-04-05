/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/reading_list/features/reading_list_switches.h"

#define BRAVE_TAB_STRIP_MODEL_DEACTIVATE_READING_LIST \
  if (true)                                           \
    return false;

#include "src/chrome/browser/ui/tabs/tab_strip_model.cc"
#undef BRAVE_TAB_STRIP_MODEL_DEACTIVATE_READING_LIST
