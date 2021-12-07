/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_14.h"

#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

namespace {

const char kSQL[] = R"sql(
  UPDATE promotion SET approximate_value = (SELECT (suggestions * 0.25)
  FROM promotion as ps WHERE ps.promotion_id = promotion.promotion_id);

  UPDATE unblinded_tokens SET value = 0.25;
)sql";

}  // namespace

constexpr int Upgrade14::kVersion;

void Upgrade14::Start() {
  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion, kSQL));
}

}  // namespace ledger
