/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_SIGNATURE_H_  // NOLINT
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_SIGNATURE_H_  // NOLINT

#include <string>

#include "wrapper.hpp"

namespace ads {
namespace privacy {

class VerificationSignature {
 public:
  VerificationSignature();
  VerificationSignature(
      const std::string& verification_signature_base64);
  ~VerificationSignature();

  bool operator==(const VerificationSignature& rhs);
  bool operator!=(const VerificationSignature& rhs);

  challenge_bypass_ristretto::VerificationSignature get() const;

  std::string EncodeBase64() const;

 private:
  challenge_bypass_ristretto::VerificationSignature verification_signature_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_SIGNATURE_H_  // NOLINT
