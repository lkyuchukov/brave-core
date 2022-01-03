/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_KEY_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_KEY_H_

#include <string>

#include "wrapper.hpp"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/verification_signature.h"

namespace ads {
namespace privacy {

class VerificationKey {
 public:
  VerificationKey();
  VerificationKey(
      const std::string& verification_key_base64);
  ~VerificationKey();

  bool operator==(const VerificationKey& rhs);
  bool operator!=(const VerificationKey& rhs);

  static VerificationSignature Sign(
      const std::string& value);

  bool Verify(
      const VerificationSignature& verification_signature,
      const std::string& value) const;

 private:
  challenge_bypass_ristretto::VerificationKey verification_key_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_VERIFICATION_KEY_H_  // NOLINT
