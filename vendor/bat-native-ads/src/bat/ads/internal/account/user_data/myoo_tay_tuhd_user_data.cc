/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/account/user_data/myoo_tay_tuhd_user_data.h"

#include <utility>

#include "base/values.h"
#include "bat/ads/internal/account/confirmations/confirmations_state.h"
#include "bat/ads/internal/client/client.h"

namespace ads {
namespace user_data {

namespace {
constexpr char kMyooTayTuhdKey[] = "mutated";
}  // namespace

base::DictionaryValue GetMyooTayTuhd() {
  base::DictionaryValue user_data;

  if (ConfirmationsState::Get()->is_myoo_tay_tuhd() ||
      Client::Get()->is_myoo_tay_tuhd()) {
    user_data.SetBoolKey(kMyooTayTuhdKey, true);
  }

  return user_data;
}

}  // namespace user_data
}  // namespace ads
