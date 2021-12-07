/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_35.h"

#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/upgrades/migration_job.h"

namespace ledger {

constexpr int Upgrade35::kVersion;

void Upgrade35::Start() {
  context().GetLedgerImpl()->state()->Initialize(
      ContinueWithLambda(this, &Upgrade35::OnStateInitialized));
}

void Upgrade35::OnStateInitialized(mojom::Result result) {
  if (result != mojom::Result::LEDGER_OK)
    return Complete(false);

  CompleteWithFuture(context().StartJob<MigrationJob>(kVersion));
}

}  // namespace ledger
