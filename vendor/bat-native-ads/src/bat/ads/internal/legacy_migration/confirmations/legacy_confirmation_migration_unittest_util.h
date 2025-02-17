/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_LEGACY_MIGRATION_CONFIRMATIONS_LEGACY_CONFIRMATION_MIGRATION_UNITTEST_UTIL_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_LEGACY_MIGRATION_CONFIRMATIONS_LEGACY_CONFIRMATION_MIGRATION_UNITTEST_UTIL_H_

#include <cstdint>

namespace ads {
namespace confirmations {

void Migrate(const bool should_migrate);

uint64_t GetHash();
void SetHash(const uint64_t hash);

}  // namespace confirmations
}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_LEGACY_MIGRATION_CONFIRMATIONS_LEGACY_CONFIRMATION_MIGRATION_UNITTEST_UTIL_H_
