/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_SIGNED_TOKEN_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_SIGNED_TOKEN_H_

#include <string>

#include "wrapper.hpp"

namespace ads {
namespace privacy {

class SignedToken {
 public:
  SignedToken();
  SignedToken(
      const std::string& signed_token_base64);
  ~SignedToken();

  bool operator==(const SignedToken& rhs);
  bool operator!=(const SignedToken& rhs);

  challenge_bypass_ristretto::SignedToken get() const;

  std::string EncodeBase64() const;

 private:
  challenge_bypass_ristretto::SignedToken signed_token_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_SIGNED_TOKEN_H_
