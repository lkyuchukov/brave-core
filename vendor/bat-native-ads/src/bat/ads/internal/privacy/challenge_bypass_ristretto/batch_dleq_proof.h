/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_BATCH_DLEQ_PROOF_H_
#define BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_BATCH_DLEQ_PROOF_H_

#include <string>
#include <vector>

#include "wrapper.hpp"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/blinded_token.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/public_key.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/signed_token.h"
#include "bat/ads/internal/privacy/challenge_bypass_ristretto/token.h"

namespace ads {
namespace privacy {

class BatchDLEQProof {
 public:
  BatchDLEQProof();
  BatchDLEQProof(
      const std::string& batch_dleq_proof_base64);
  ~BatchDLEQProof();

  bool operator==(const BatchDLEQProof& rhs);
  bool operator!=(const BatchDLEQProof& rhs);

  challenge_bypass_ristretto::BatchDLEQProof get() const;

  std::string EncodeBase64() const;

  bool Verify(
      const std::vector<BlindedToken>& blinded_tokens,
      const std::vector<SignedToken>& signed_tokens,
      const PublicKey& public_key);

  bool VerifyAndUnblind(
      const std::vector<Token>& tokens,
      const std::vector<BlindedToken>& blinded_tokens,
      const std::vector<SignedToken>& signed_tokens,
      const PublicKey& public_key);

 private:
  challenge_bypass_ristretto::BatchDLEQProof batch_dleq_proof_;
}

}  // namespace privacy
}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PRIVACY_CHALLENGE_BYPASS_RISTRETTO_BATCH_DLEQ_PROOF_H_  // NOLINT
