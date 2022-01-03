/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/verification_key.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

VerificationKey::VerificationKey() = default;

VerificationKey::VerificationKey(
    const std::string& verification_key_base64) {
  verification_key_ = challenge_bypass_ristretto::VerificationKey
      ::decode_base64(verification_key_base64);
}

VerificationKey::~VerificationKey() = default;

bool VerificationKey::operator==(
    const VerificationKey& rhs) const {
  return verification_key_ == rhs.get();
}

bool VerificationKey::operator!=(
    const VerificationKey& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::VerificationKey get() const {
  return verification_key_;
}

VerificationSignature VerificationKey::Sign(
    const std::string& value) {
  challenge_bypass_ristretto::VerificationSignature raw_verification_signature =
      verification_key_.sign(value);

  const std::string verification_signature_base64 =
      raw_verification_signature.encode_base64();

  VerificationSignature verification_signature(verification_signature_base64);
  return verification_signature;
}

bool VerificationKey::Verify(
    const VerificationSignature& verification_signature,
    const std::string& value) const {
  return verification_key_.verify(verification_signature.get(), value);
}

}  // namespace privacy
}  // namespace ads
