/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/blinded_token.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

BlindedToken::BlindedToken() = default;

BlindedToken::BlindedToken(
    const std::string& blinded_token_base64) {
  blinded_token_ = challenge_bypass_ristretto::BlindedToken::decode_base64(
      blinded_token_base64);
}

BlindedToken::~BlindedToken() = default;

challenge_bypass_ristretto::BlindedToken get() const {
  return blinded_token_;
}

std::string BlindedToken::EncodeBase64() const {
  return blinded_token_.encode_base64();
}

}  // namespace privacy
}  // namespace ads
