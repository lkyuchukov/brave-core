/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_34.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

// Migration 34 adds a "claimable_until" field to the promotion database so that
// a "days to claim" countdown can be displayed to the user.
const char kSQL[] = R"sql(
  ALTER TABLE promotion ADD COLUMN claimable_until INTEGER;
)sql";

}  // namespace

constexpr int Upgrade34::kVersion;

void Upgrade34::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
