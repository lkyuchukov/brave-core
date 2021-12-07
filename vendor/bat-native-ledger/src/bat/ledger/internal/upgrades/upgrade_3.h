/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_UPGRADES_UPGRADE_3_H_
#define BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_UPGRADES_UPGRADE_3_H_

#include "bat/ledger/internal/core/bat_ledger_job.h"

namespace ledger {

class Upgrade3 : public BATLedgerJob<bool> {
 public:
  static constexpr int kVersion = 3;
  void Start();
};

}  // namespace ledger

#endif  // BRAVE_VENDOR_BAT_NATIVE_LEDGER_SRC_BAT_LEDGER_INTERNAL_UPGRADES_UPGRADE_3_H_
