/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/upgrades/upgrade_manager.h"

#include <algorithm>
#include <map>
#include <string>

#include "bat/ledger/internal/core/bat_ledger_job.h"
#include "bat/ledger/internal/core/sql_store.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/upgrades/upgrade_1.h"
#include "bat/ledger/internal/upgrades/upgrade_10.h"
#include "bat/ledger/internal/upgrades/upgrade_11.h"
#include "bat/ledger/internal/upgrades/upgrade_12.h"
#include "bat/ledger/internal/upgrades/upgrade_13.h"
#include "bat/ledger/internal/upgrades/upgrade_14.h"
#include "bat/ledger/internal/upgrades/upgrade_15.h"
#include "bat/ledger/internal/upgrades/upgrade_16.h"
#include "bat/ledger/internal/upgrades/upgrade_17.h"
#include "bat/ledger/internal/upgrades/upgrade_18.h"
#include "bat/ledger/internal/upgrades/upgrade_19.h"
#include "bat/ledger/internal/upgrades/upgrade_2.h"
#include "bat/ledger/internal/upgrades/upgrade_20.h"
#include "bat/ledger/internal/upgrades/upgrade_21.h"
#include "bat/ledger/internal/upgrades/upgrade_22.h"
#include "bat/ledger/internal/upgrades/upgrade_23.h"
#include "bat/ledger/internal/upgrades/upgrade_24.h"
#include "bat/ledger/internal/upgrades/upgrade_25.h"
#include "bat/ledger/internal/upgrades/upgrade_26.h"
#include "bat/ledger/internal/upgrades/upgrade_27.h"
#include "bat/ledger/internal/upgrades/upgrade_28.h"
#include "bat/ledger/internal/upgrades/upgrade_29.h"
#include "bat/ledger/internal/upgrades/upgrade_3.h"
#include "bat/ledger/internal/upgrades/upgrade_30.h"
#include "bat/ledger/internal/upgrades/upgrade_31.h"
#include "bat/ledger/internal/upgrades/upgrade_32.h"
#include "bat/ledger/internal/upgrades/upgrade_33.h"
#include "bat/ledger/internal/upgrades/upgrade_34.h"
#include "bat/ledger/internal/upgrades/upgrade_35.h"
#include "bat/ledger/internal/upgrades/upgrade_4.h"
#include "bat/ledger/internal/upgrades/upgrade_5.h"
#include "bat/ledger/internal/upgrades/upgrade_6.h"
#include "bat/ledger/internal/upgrades/upgrade_7.h"
#include "bat/ledger/internal/upgrades/upgrade_8.h"
#include "bat/ledger/internal/upgrades/upgrade_9.h"

namespace ledger {

namespace {

using UpgradeHandler = Future<bool> (*)(BATLedgerContext* context);

template <typename T>
Future<bool> UpgradeHandlerFor(BATLedgerContext* context) {
  return context->StartJob<T>();
}

template <typename... Ts>
std::map<int, UpgradeHandler> CreateUpgradeHandlerMap() {
  return {{Ts::kVersion, UpgradeHandlerFor<Ts>}...};
}

class UpgradeJob : public BATLedgerJob<bool> {
 public:
  UpgradeJob() {
    for (auto& pair : upgrade_handlers_) {
      current_version_ = std::max(current_version_, pair.first);
    }
  }

  void Start() {
    context()
        .Get<SQLStore>()
        .Open(current_version_)
        .Then(ContinueWith(this, &UpgradeJob::OnDatabaseOpened));
  }

  void Start(int target_version) {
    current_version_ = target_version;
    Start();
  }

 private:
  void OnDatabaseOpened(SQLReader reader) {
    if (!reader.Succeeded()) {
      context().LogError(FROM_HERE) << "Unable to open database";
      Complete(false);
    }

    if (reader.Step()) {
      db_version_ = static_cast<int>(reader.ColumnInt64(0));
      starting_version_ = db_version_;
    }

    RunNextUpgrade();
  }

  void RunNextUpgrade() {
    if (db_version_ == current_version_) {
      return MaybeVacuumDatabase();
    }

    int next = db_version_ + 1;

    auto iter = upgrade_handlers_.find(next);
    if (iter != upgrade_handlers_.end()) {
      context().LogVerbose(FROM_HERE) << "Upgrading to version " << next;
      auto& handler = iter->second;
      handler(&context())
          .Then(ContinueWith(this, &UpgradeJob::OnUpgradeHandlerComplete));
    } else {
      OnUpgradeHandlerComplete(true);
    }
  }

  void OnUpgradeHandlerComplete(bool success) {
    if (!success) {
      context().LogError(FROM_HERE) << "Upgrade " << db_version_ << "failed";
      return Complete(false);
    }
    db_version_ += 1;
    RunNextUpgrade();
  }

  void MaybeVacuumDatabase() {
    if (starting_version_ < db_version_) {
      context().LogVerbose(FROM_HERE) << "Freeing unused space in database";
      context().Get<SQLStore>().Vacuum().Then(
          ContinueWith(this, &UpgradeJob::OnDatabaseVacuumComplete));
    } else {
      Complete(true);
    }
  }

  void OnDatabaseVacuumComplete(SQLReader reader) {
    if (!reader.Succeeded()) {
      context().LogError(FROM_HERE) << "Database vacuum failed";
    }
    Complete(true);
  }

  int starting_version_ = 0;
  int db_version_ = 0;
  int current_version_ = 0;

  std::map<int, UpgradeHandler> upgrade_handlers_ =
      CreateUpgradeHandlerMap<Upgrade1,
                              Upgrade2,
                              Upgrade3,
                              Upgrade4,
                              Upgrade5,
                              Upgrade6,
                              Upgrade7,
                              Upgrade8,
                              Upgrade9,
                              Upgrade10,
                              Upgrade11,
                              Upgrade12,
                              Upgrade13,
                              Upgrade14,
                              Upgrade15,
                              Upgrade16,
                              Upgrade17,
                              Upgrade18,
                              Upgrade19,
                              Upgrade20,
                              Upgrade21,
                              Upgrade22,
                              Upgrade23,
                              Upgrade24,
                              Upgrade25,
                              Upgrade26,
                              Upgrade27,
                              Upgrade28,
                              Upgrade29,
                              Upgrade30,
                              Upgrade31,
                              Upgrade32,
                              Upgrade33,
                              Upgrade34,
                              Upgrade35>();
};

}  // namespace

Future<bool> UpgradeManager::Initialize() {
  return context().StartJob<UpgradeJob>();
}

Future<bool> UpgradeManager::UpgradeToVersionForTesting(int version) {
  if (version <= 0) {
    return context().StartJob<UpgradeJob>();
  }

  return context().StartJob<UpgradeJob>(version);
}

}  // namespace ledger
