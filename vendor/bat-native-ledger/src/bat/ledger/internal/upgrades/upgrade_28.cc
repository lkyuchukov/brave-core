/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_28.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  DELETE FROM server_publisher_info
  WHERE status = 0 OR publisher_key NOT IN (
    SELECT publisher_id FROM publisher_info
  );

  ALTER TABLE server_publisher_info RENAME TO server_publisher_info_temp;

  CREATE TABLE server_publisher_info (
    publisher_key LONGVARCHAR PRIMARY KEY NOT NULL,
    status INTEGER DEFAULT 0 NOT NULL,
    address TEXT NOT NULL,
    updated_at TIMESTAMP NOT NULL
  );

  INSERT OR IGNORE INTO server_publisher_info
    (publisher_key, status, address, updated_at)
  SELECT publisher_key, status, address, 0
  FROM server_publisher_info_temp;

  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS server_publisher_info_temp;
  PRAGMA foreign_keys = on;

  DELETE FROM server_publisher_banner
  WHERE publisher_key NOT IN (SELECT publisher_key FROM server_publisher_info);

  DELETE FROM server_publisher_links
  WHERE publisher_key NOT IN (SELECT publisher_key FROM server_publisher_info);

  DELETE FROM server_publisher_amounts
  WHERE publisher_key NOT IN (SELECT publisher_key FROM server_publisher_info);

  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS publisher_prefix_list;
  PRAGMA foreign_keys = on;

  CREATE TABLE publisher_prefix_list (hash_prefix BLOB PRIMARY KEY NOT NULL);
)sql";

}  // namespace

constexpr int Upgrade28::kVersion;

void Upgrade28::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
