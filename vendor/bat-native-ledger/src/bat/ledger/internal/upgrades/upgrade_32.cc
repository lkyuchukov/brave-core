/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_32.h"

#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/upgrades/migration_job.h"
#include "bat/ledger/option_keys.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  CREATE TABLE balance_report_info_bap AS SELECT * FROM balance_report_info;
  DELETE FROM balance_report_info;
)sql";

}  // namespace

constexpr int Upgrade32::kVersion;

void Upgrade32::Start() {
  // Migration 32 archives and clears additional data associated with BAP in
  // order to prevent display of BAP historical information in monthly reports.
  bool is_bitflyer_region =
      context().GetLedgerClient()->GetBooleanOption(option::kIsBitflyerRegion);

  const char* sql = is_bitflyer_region ? kSQL : "";
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, sql));
}

}  // namespace ledger
