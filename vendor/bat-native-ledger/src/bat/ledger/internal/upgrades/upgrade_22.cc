/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_22.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS balance_report_info;
  PRAGMA foreign_keys = on;

  CREATE TABLE balance_report_info (
    balance_report_id LONGVARCHAR PRIMARY KEY NOT NULL,
    grants_ugp DOUBLE DEFAULT 0 NOT NULL,
    grants_ads DOUBLE DEFAULT 0 NOT NULL,
    auto_contribute DOUBLE DEFAULT 0 NOT NULL,
    tip_recurring DOUBLE DEFAULT 0 NOT NULL,
    tip DOUBLE DEFAULT 0 NOT NULL
  );

  CREATE INDEX balance_report_info_balance_report_id_index
    ON balance_report_info (balance_report_id);

  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS processed_publisher;
  PRAGMA foreign_keys = on;

  CREATE TABLE processed_publisher (
    publisher_key TEXT PRIMARY KEY NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
  );
)sql";

}  // namespace

constexpr int Upgrade22::kVersion;

void Upgrade22::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
