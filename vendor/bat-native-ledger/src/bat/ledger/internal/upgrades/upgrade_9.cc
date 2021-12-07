/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_9.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS contribution_queue;
  PRAGMA foreign_keys = on;

  CREATE TABLE contribution_queue (
    contribution_queue_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    type INTEGER NOT NULL,
    amount DOUBLE NOT NULL,
    partial INTEGER NOT NULL DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL
  );

  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS contribution_queue_publishers;
  PRAGMA foreign_keys = on;

  CREATE TABLE contribution_queue_publishers (
    contribution_queue_id INTEGER NOT NULL,
    publisher_key TEXT NOT NULL,
    amount_percent DOUBLE NOT NULL,
    CONSTRAINT fk_contribution_queue_publishers_publisher_key
      FOREIGN KEY (publisher_key)
      REFERENCES publisher_info (publisher_id),
    CONSTRAINT fk_contribution_queue_publishers_id
      FOREIGN KEY (contribution_queue_id)
      REFERENCES contribution_queue (contribution_queue_id)
      ON DELETE CASCADE
  );
)sql";

}  // namespace

constexpr int Upgrade9::kVersion;

void Upgrade9::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
