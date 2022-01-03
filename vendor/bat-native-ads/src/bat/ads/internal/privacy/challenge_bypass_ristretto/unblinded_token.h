/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_H_

#include <string>

#include "wrapper.hpp"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/verification_key.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/token_preimage.h"

namespace ads {
namespace privacy {

class UnblindedToken {
 public:
  UnblindedToken();
  UnblindedToken(
      const std::string& unblinded_token_base64);
  ~UnblindedToken();

  bool operator==(const UnblindedToken& rhs);
  bool operator!=(const UnblindedToken& rhs);

  challenge_bypass_ristretto::UnblindedToken get() const;

  std::string EncodeBase64() const;

  VerificationKey DeriveVerificationKey() const;

  TokenPreimage TokenPreimage() const;

 private:
  challenge_bypass_ristretto::UnblindedToken unblinded_token_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_UNBLINDED_TOKEN_H_  // NOLINT
