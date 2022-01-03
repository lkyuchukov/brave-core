/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/signing_key.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

SigningKey::SigningKey() = default;

SigningKey::SigningKey(
    const std::string& signing_key_base64) {
  signing_key_ = challenge_bypass_ristretto::SigningKey::decode_base64(
      signing_key_base64);
}

SigningKey::~SigningKey() = default;

bool SigningKey::operator==(
    const SigningKey& rhs) const {
  return signing_key_ == rhs.get();
}

bool SigningKey::operator!=(
    const SigningKey& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::SigningKey get() const {
  return signing_key_;
}

SigningKey SigningKey::Random() const {
  challenge_bypass_ristretto::SigningKey raw_signing_key =
      challenge_bypass_ristretto::SigningKey::Random();

  const std::string signing_key_base64 = raw_signing_key.encode_base64();

  SigningKey signing_key(signing_key_base64);
  return signing_key;
}

SignedToken SigningKey::Sign(
    const BlindedToken& blinded_token) const {
  challenge_bypass_ristretto::SignedToken raw_signed_token =
      signing_key_.sign(blinded_token.get());

  const std::string signed_token_base64 = raw_signed_token.encode_base64();

  SignedToken signed_token(signed_token_base64);
  return signed_token;
}

UnblindedToken SigningKey::RederiveUnblindedToken(
    const TokenPreimage& token_preimage) const {
  challenge_bypass_ristretto::UnblindedToken raw_unblinded_token =
      signing_key_.rederive_unblinded_token(blinded_token.get());

  challenge_bypass_ristretto::TokenPreimage raw_token_preimage =
      raw_unblinded_token.preimage();

  challenge_bypass_ristretto::UnblindedToken raw_derevived_unblinded_token =
      raw_unblinded_token.derive_verification_key(raw_token_preimage);

  const std::string rederived_unblinded_token_base64 =
      raw_derevived_unblinded_token.encode_base64();

  UnblindedToken unblinded_token(rederived_unblinded_token_base64);
  return unblinded_token;
}

PublicKey SigningKey::PublicKey() const {
  challenge_bypass_ristretto::PublicKey raw_public_key =
      unblinded_token_.public_key();

  const std::string public_key_base64 = raw_public_key.encode_base64();

  PublicKey public_key(public_key_base64);
  return public_key;
}

}  // namespace privacy
}  // namespace ads
