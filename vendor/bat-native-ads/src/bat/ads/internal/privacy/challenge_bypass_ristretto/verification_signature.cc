/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/verification_signature.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

VerificationSignature::VerificationSignature() = default;

VerificationSignature::VerificationSignature(
    const std::string& verification_signature_base64) {
  verification_signature_ = challenge_bypass_ristretto::VerificationSignature
      ::decode_base64(verification_signature_base64);
}

VerificationSignature::~VerificationSignature() = default;

bool VerificationSignature::operator==(
    const VerificationSignature& rhs) const {
  return verification_signature_ == rhs.get();
}

bool VerificationSignature::operator!=(
    const VerificationSignature& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::VerificationSignature get() const {
  return verification_signature_;
}

std::string VerificationSignature::EncodeBase64() const {
  return verification_signature_.encode_base64();
}

}  // namespace privacy
}  // namespace ads
