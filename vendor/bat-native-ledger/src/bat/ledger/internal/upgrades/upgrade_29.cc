/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_29.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS event_log;
  PRAGMA foreign_keys = on;

  CREATE TABLE event_log (
    event_log_id LONGVARCHAR PRIMARY KEY NOT NULL,
    key TEXT NOT NULL,
    value TEXT NOT NULL,
    created_at TIMESTAMP NOT NULL
  );
)sql";

}  // namespace

constexpr int Upgrade29::kVersion;

void Upgrade29::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
