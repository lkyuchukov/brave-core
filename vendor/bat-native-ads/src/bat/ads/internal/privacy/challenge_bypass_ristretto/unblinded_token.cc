/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/unblinded_token.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

UnblindedToken::UnblindedToken() = default;

UnblindedToken::UnblindedToken(
    const std::string& unblinded_token_base64) {
  unblinded_token_ = challenge_bypass_ristretto::UnblindedToken
      ::decode_base64(unblinded_token_base64);
}

UnblindedToken::~UnblindedToken() = default;

bool UnblindedToken::operator==(
    const UnblindedToken& rhs) const {
  return unblinded_token_ == rhs.get();
}

bool UnblindedToken::operator!=(
    const UnblindedToken& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::UnblindedToken get() const {
  return unblinded_token_;
}

std::string UnblindedToken::EncodeBase64() const {
  return unblinded_token_.encode_base64();
}

VerificationKey UnblindedToken::DeriveVerificationKey() const {
  challenge_bypass_ristretto::VerificationKey raw_verification_key =
      unblinded_token_.derive_verification_key();

  const std::string verification_key_base64 =
      raw_verification_key.encode_base64();

  VerificationKey verification_key(verification_key_base64);
  return verification_key;
}

TokenPreimage VerificationKey::TokenPreimage() const {
  challenge_bypass_ristretto::TokenPreimage raw_token_preimage =
      unblinded_token_.preimage();

  const std::string preimage_base64 = raw_token_preimage.encode_base64();

  TokenPreimage token_preimage(preimage_base64);
  return token_preimage;
}

}  // namespace privacy
}  // namespace ads
