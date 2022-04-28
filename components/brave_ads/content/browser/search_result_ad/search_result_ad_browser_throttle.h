/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_THROTTLE_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_THROTTLE_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "third_party/blink/public/common/loader/url_loader_throttle.h"

namespace brave_ads {

class SearchResultAdService;

class SearchResultAdThrottle : public blink::URLLoaderThrottle {
 public:
  explicit SearchResultAdThrottle(SearchResultAdService* search_result_ad_service);
  ~SearchResultAdThrottle() override;

  SearchResultAdThrottle(const SearchResultAdThrottle&) = delete;
  SearchResultAdThrottle& operator=(const SearchResultAdThrottle&) = delete;

  static std::unique_ptr<SearchResultAdThrottle> MaybeCreateThrottleFor(
      SearchResultAdService* search_result_ad_service);

  // Implements blink::URLLoaderThrottle.
  void WillStartRequest(network::ResourceRequest* request,
                        bool* defer) override;

 private:
  raw_ptr<SearchResultAdService> search_result_ad_service_ = nullptr;

  base::WeakPtrFactory<SearchResultAdThrottle> weak_factory_{this};
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_THROTTLE_H_
