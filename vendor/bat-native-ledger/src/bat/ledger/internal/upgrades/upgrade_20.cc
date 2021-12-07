/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_20.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  DROP INDEX IF EXISTS unblinded_tokens_creds_id_index;

  ALTER TABLE unblinded_tokens ADD redeemed_at TIMESTAMP NOT NULL DEFAULT 0;

  ALTER TABLE unblinded_tokens ADD redeem_id TEXT;

  ALTER TABLE unblinded_tokens ADD redeem_type INTEGER NOT NULL DEFAULT 0;

  CREATE INDEX unblinded_tokens_creds_id_index ON unblinded_tokens (creds_id);

  CREATE INDEX unblinded_tokens_redeem_id_index ON unblinded_tokens (redeem_id);
)sql";

}  // namespace

constexpr int Upgrade20::kVersion;

void Upgrade20::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
