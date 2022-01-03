/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/privacy/challenge_bypass_ristretto/token.h"

#include "bat/ads/internal/logging.h"

namespace ads {
namespace privacy {

Token::Token() = default;

Token::Token(
    const std::string& token_base64) {
  token_ = challenge_bypass_ristretto::Token::decode_base64(token_base64);
}

Token::~Token() = default;

bool Token::operator==(
    const Token& rhs) const {
  return token_ == rhs.get();
}

bool Token::operator!=(
    const Token& rhs) const {
  return !(*this == rhs);
}

challenge_bypass_ristretto::Token get() const {
  return token_;
}

std::string Token::EncodeBase64() const {
  return token_.encode_base64();
}

Token Token::Random() {
  challenge_bypass_ristretto::Token raw_token =
      challenge_bypass_ristretto::Token::Random();

  const std::string token_base64 = raw_token.encode_base64();

  Token token(token_base64);
  return token;
}

BlindedToken Token::Blind() const {
  challenge_bypass_ristretto::TokenPreimage raw_blinded_token =
      unblinded_token_.blind();

  const std::string blinded_token_base64 = raw_blinded_token.encode_base64();

  BlindedToken blinded_token(blinded_token_base64);
  return blinded_token;
}

}  // namespace privacy
}  // namespace ads
