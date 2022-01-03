/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_DLEQ_PROOF_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_DLEQ_PROOF_H_

#include <string>

#include "wrapper.hpp"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/blinded_token.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/public_key.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/signed_token.h"

namespace ads {
namespace privacy {

class DLEQProof {
 public:
  DLEQProof();
  DLEQProof(
      const std::string& dleq_proof_base64);
  ~DLEQProof();

  bool operator==(const DLEQProof& rhs);
  bool operator!=(const DLEQProof& rhs);

  challenge_bypass_ristretto::DLEQProof get() const;

  std::string EncodeBase64() const;

  bool Verify(
      const BlindedToken& blinded_token,
      const SignedToken& signed_token,
      const PublicKey& public_key) const;

 private:
  challenge_bypass_ristretto::DLEQProof dleq_proof_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_DLEQ_PROOF_H_
