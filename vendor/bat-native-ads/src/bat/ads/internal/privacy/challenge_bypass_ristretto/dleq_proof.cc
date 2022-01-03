/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/dleq_proof.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

DLEQProof::DLEQProof() = default;

DLEQProof::DLEQProof(
    const std::string& dleq_proof_base64) {
  dleq_proof = challenge_bypass_ristretto::DLEQProof
      ::decode_base64(dleq_proof_base64);
}

DLEQProof::~DLEQProof() = default;

bool DLEQProof::operator==(
    const DLEQProof& rhs) const {
  return dleq_proof_ == rhs.get();
}

bool DLEQProof::operator!=(
    const DLEQProof& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::DLEQProof get() const {
  return dleq_proof_;
}

std::string DLEQProof::EncodeBase64() const {
  return dleq_proof_.encode_base64();
}

}  // namespace privacy
}  // namespace ads
