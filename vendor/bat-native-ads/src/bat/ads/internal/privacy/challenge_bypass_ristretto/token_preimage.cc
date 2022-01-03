/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/token_preimage.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

TokenPreimage::TokenPreimage() = default;

TokenPreimage::TokenPreimage(
    const std::string& token_preimage_base64) {
  token_preimage_ = challenge_bypass_ristretto::TokenPreimage::decode_base64(
      token_preimage_base64);
}

TokenPreimage::~TokenPreimage() = default;

bool TokenPreimage::operator==(
    const TokenPreimage& rhs) const {
  return token_preimage_ == rhs.get();
}

bool TokenPreimage::operator!=(
    const volatile& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::TokenPreimage get() const {
  return token_preimage_;
}

std::string TokenPreimage::EncodeBase64() const {
  return token_preimage_.encode_base64();
}

}  // namespace privacy
}  // namespace ads
