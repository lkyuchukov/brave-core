/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_TOKEN_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_TOKEN_H_

#include <string>

#include "wrapper.hpp"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/blinded_token.h"

namespace ads {
namespace privacy {

class Token {
 public:
  Token();
  Token(
      const std::string& token_base64);
  ~Token();

  bool operator==(const Token& rhs);
  bool operator!=(const Token& rhs);

  challenge_bypass_ristretto::Token get() const;

  std::string EncodeBase64() const;

  Token Random() const;

  BlindedToken Blind() const;

 private:
  challenge_bypass_ristretto::Token token_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_TOKEN_H_
