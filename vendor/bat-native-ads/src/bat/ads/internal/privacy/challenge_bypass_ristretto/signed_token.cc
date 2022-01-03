/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/signed_token.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

SignedToken::SignedToken() = default;

SignedToken::SignedToken(
    const std::string& signed_token_base64) {
  signed_token_ = challenge_bypass_ristretto::SignedToken::decode_base64(
      signed_token_base64);
}

SignedToken::~SignedToken() = default;

challenge_bypass_ristretto::SignedToken get() const {
  return signed_token_;
}

std::string SignedToken::EncodeBase64() const {
  return signed_token_.encode_base64();
}

}  // namespace privacy
}  // namespace ads
