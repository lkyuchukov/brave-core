/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_PUBLIC_KEY_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_PUBLIC_KEY_H_

#include <string>

#include "wrapper.hpp"

namespace ads {
namespace privacy {

class PublicKey {
 public:
  PublicKey();
  PublicKey(
      const std::string& public_key_base64);
  ~PublicKey();

  bool operator==(const PublicKey& rhs);
  bool operator!=(const PublicKey& rhs);

  challenge_bypass_ristretto::PublicKey get() const;

  std::string EncodeBase64() const;

 private:
  challenge_bypass_ristretto::PublicKey public_key_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_PUBLIC_KEY_H_
