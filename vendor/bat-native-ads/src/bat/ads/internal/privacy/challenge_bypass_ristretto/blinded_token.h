/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_BLINDED_TOKEN_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_BLINDED_TOKEN_H_

#include <string>

#include "wrapper.hpp"

namespace ads {
namespace privacy {

class BlindedToken {
 public:
  BlindedToken();
  BlindedToken(
      const std::string& blinded_token_base64);
  ~BlindedToken();

  bool operator==(const BlindedToken& rhs);
  bool operator!=(const BlindedToken& rhs);

  challenge_bypass_ristretto::BlindedToken get() const;

  std::string EncodeBase64() const;

 private:
  challenge_bypass_ristretto::BlindedToken blinded_token_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_BLINDED_TOKEN_H_
