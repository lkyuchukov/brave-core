/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/account/issuers/issuers_json_reader_util.h"

#include "bat/ads/internal/account/issuers/issuers_value_util.h"

namespace ads {
namespace JSONReader {

namespace {

constexpr char kPingKey[] = "ping";
constexpr char kIssuersKey[] = "issuers";

}  // namespace

absl::optional<int> ParsePing(const base::Value& value) {
  return value.FindIntKey(kPingKey);
}

absl::optional<IssuerList> ParseIssuers(const base::Value::Dict& value) {
  const base::Value::List* issuers_value = value.FindList(kIssuersKey);
  if (!issuers_value) {
    return absl::nullopt;
  }

  const absl::optional<IssuerList>& issuers_optional =
      ValueToIssuerList(*issuers_value);
  if (!issuers_optional) {
    return absl::nullopt;
  }

  return issuers_optional;
}

}  // namespace JSONReader
}  // namespace ads
