/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_SIGNING_KEY_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_SIGNING_KEY_H_

#include <string>

#include "wrapper.hpp"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/blinded_token.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/public_key.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/signed_token.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/unblinded_token.h"

namespace ads {
namespace privacy {

class SigningKey {
 public:
  SigningKey();
  SigningKey(
      const std::string& signing_key_base64);
  ~SigningKey();

  bool operator==(const SigningKey& rhs);
  bool operator!=(const SigningKey& rhs);

  challenge_bypass_ristretto::SigningKey get() const;

  std::string EncodeBase64() const;

  SigningKey Random() const;

  SignedToken Sign(
      const BlindedToken& blinded_token) const;

  UnblindedToken RederiveUnblindedToken(
      const TokenPreimage& token_preimage) const;

  PublicKey PublicKey() const;

 private:
  challenge_bypass_ristretto::SigningKey signing_key_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_SIGNING_KEY_H_
