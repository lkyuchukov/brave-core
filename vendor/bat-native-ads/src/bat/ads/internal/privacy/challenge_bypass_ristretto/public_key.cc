/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/public_key.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

PublicKey::PublicKey() = default;

PublicKey::PublicKey(
    const std::string& public_key_base64) {
  public_key_ = challenge_bypass_ristretto::PublicKey
      ::decode_base64(public_key_base64);
}

PublicKey::~PublicKey() = default;

bool PublicKey::operator==(
    const PublicKey& rhs) const {
  return public_key_ == rhs.get();
}

bool PublicKey::operator!=(
    const PublicKey& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::PublicKey get() const {
  return public_key_;
}

std::string PublicKey::EncodeBase64() const {
  return public_key_.encode_base64();
}

}  // namespace privacy
}  // namespace ads
