/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_1.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS activity_info;
  PRAGMA foreign_keys = on;

  CREATE TABLE activity_info (
    publisher_id LONGVARCHAR NOT NULL,
    duration INTEGER DEFAULT 0 NOT NULL,
    score DOUBLE DEFAULT 0 NOT NULL,
    percent INTEGER DEFAULT 0 NOT NULL,
    weight DOUBLE DEFAULT 0 NOT NULL,
    category INTEGER NOT NULL,
    month INTEGER NOT NULL,
    year INTEGER NOT NULL,
    CONSTRAINT fk_activity_info_publisher_id
      FOREIGN KEY (publisher_id)
      REFERENCES publisher_info (publisher_id)
      ON DELETE CASCADE
  );

  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS media_publisher_info;
  PRAGMA foreign_keys = on;

  CREATE TABLE media_publisher_info (
    media_key TEXT NOT NULL PRIMARY KEY UNIQUE,
    publisher_id LONGVARCHAR NOT NULL,
    CONSTRAINT fk_media_publisher_info_publisher_id
      FOREIGN KEY (publisher_id)
      REFERENCES publisher_info (publisher_id)
      ON DELETE CASCADE
  );

  PRAGMA foreign_keys = off;
    DROP TABLE IF EXISTS publisher_info;
  PRAGMA foreign_keys = on;

  CREATE TABLE publisher_info (
    publisher_id LONGVARCHAR PRIMARY KEY NOT NULL UNIQUE,
    verified BOOLEAN DEFAULT 0 NOT NULL,
    excluded INTEGER DEFAULT 0 NOT NULL,
    name TEXT NOT NULL,
    favIcon TEXT NOT NULL,
    url TEXT NOT NULL,
    provider TEXT NOT NULL
  );
)sql";

}  // namespace

constexpr int Upgrade1::kVersion;

void Upgrade1::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
