/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/batch_dleq_proof.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

BatchDLEQProof::BatchDLEQProof() = default;

BatchDLEQProof::BatchDLEQProof(
    const std::string& batch_dleq_proof_base64) {
  batch_dleq_proof_ = challenge_bypass_ristretto::BatchDLEQProof
      ::decode_base64(batch_dleq_proof_base64);
}

BatchDLEQProof::~BatchDLEQProof() = default;

bool BatchDLEQProof::operator==(
    const BatchDLEQProof& rhs) const {
  return batch_dleq_proof_ == rhs.get();
}

bool BatchDLEQProof::operator!=(
    const BatchDLEQProof& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::BatchDLEQProof get() const {
  return batch_dleq_proof_;
}

std::string BatchDLEQProof::EncodeBase64() const {
  return batch_dleq_proof.encode_base64();
}

bool BatchDLEQProof::Verify(
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const PublicKey& public_key) {
  std::vector<challenge_bypass_ristretto::BlindedToken> raw_blinded_tokens;
  for (const auto& blinded_token : blinded_tokens) {
    raw_blinded_tokens.push_back(blinded_token.get());
  }

  std::vector<challenge_bypass_ristretto::SignedToken> raw_signed_tokens;
  for (const auto& signed_token : signed_tokens) {
    raw_signed_tokens.push_back(signed_token.get());
  }

  challenge_bypass_ristretto::PublicKey raw_public_key = public.get();

  return batch_dleq_proof_.verify(raw_blinded_tokens, raw_signed_tokens,
      raw_public_key);
}

std::vector<UnblindedTokens> BatchDLEQProof::VerifyAndUnblind(
    const std::vector<Token>& tokens,
    const std::vector<BlindedToken>& blinded_tokens,
    const std::vector<SignedToken>& signed_tokens,
    const PublicKey& public_key) {
  std::vector<challenge_bypass_ristretto::Token> raw_tokens;
  for (const auto& token : tokens) {
    raw_tokens.push_back(token.get());
  }

  std::vector<challenge_bypass_ristretto::BlindedToken> raw_blinded_tokens;
  for (const auto& blinded_token : blinded_tokens) {
    raw_blinded_tokens.push_back(blinded_token.get());
  }

  std::vector<challenge_bypass_ristretto::SignedToken> raw_signed_tokens;
  for (const auto& signed_token : signed_tokens) {
    raw_signed_tokens.push_back(signed_token.get());
  }

  challenge_bypass_ristretto::PublicKey raw_public_key = public.get();

  const std::vector<challenge_bypass_ristretto::UnblindedToken>
      raw_unblinded_tokens = batch_dleq_proof_.verify_and_unblind(
          raw_tokens, raw_blinded_tokens, raw_signed_tokens, raw_public_key);

  std::vector<UnblindedToken> unblinded_tokens;
  for (const auto& raw_unblinded_token : raw_unblinded_tokens) {
    const std::string unblinded_token_base64 =
        raw_unblinded_token.encode_base64();
    UnblindedToken unblinded_token(unblinded_token_base64);
    unblinded_tokens.push_back(unblinded_token);
  }

  return unblinded_tokens;
}

}  // namespace privacy
}  // namespace ads
