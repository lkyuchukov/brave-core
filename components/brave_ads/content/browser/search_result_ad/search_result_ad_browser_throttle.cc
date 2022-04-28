/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/content/browser/search_result_ad/search_result_ad_throttle.h"

#include <memory>
#include <string>
#include <vector>

#include "brave/components/brave_ads/content/browser/search_result_ad/search_result_ad_service.h"
#include "services/network/public/cpp/request_mode.h"
#include "services/network/public/cpp/resource_request.h"
#include "url/gurl.h"

namespace brave_ads {

// static
std::unique_ptr<SearchResultAdThrottle> SearchResultAdThrottle::MaybeCreateThrottleFor(
    SearchResultAdService* search_result_ad_service) {
  if (!search_result_ad_service)
    return nullptr;
  return std::make_unique<SearchResultAdThrottle>(search_result_ad_service);
}

SearchResultAdThrottle::SearchResultAdThrottle(
    SearchResultAdService* search_result_ad_service)
    : search_result_ad_service_(search_result_ad_service) {
  DCHECK(search_result_ad_service_);
}

SearchResultAdThrottle::~SearchResultAdThrottle() = default;

void SearchResultAdThrottle::WillStartRequest(network::ResourceRequest* request,
                                        bool* defer) {
  DCHECK(search_result_ad_service_);

  LOG(ERROR) << "**************************************************** " << request->url.spec();

  GURL search_result_add_url("https://brave.com");

  const GURL url = request->url;
  if (!url.is_valid() || !url.SchemeIs(url::kHttpsScheme) ||
      url.host_piece() != "search.brave.com" || url.path_piece() != "/click" ||
      !url.has_query()) {
    LOG(ERROR) << "****************************************************";
        return;
  }

  base::StringPiece query_str = url.query_piece();
  url::Component query(0, static_cast<int>(query_str.length())), key, value;
  while (url::ExtractQueryKeyValue(query_str.data(), &query, &key, &value)) {
    base::StringPiece key_str = query_str.substr(key.begin, key.len);
    if (key_str == "id") {
      base::StringPiece value_str = query_str.substr(value.begin, value.len);
      LOG(ERROR) << "^^^^^^^^^^^^ : " << value_str;
    }
  }

  url::Origin original_origin = url::Origin::Create(request->url);
  url::Origin search_result_add_origin = url::Origin::Create(search_result_add_url);
  request->url = search_result_add_url;

  // If we're redirecting to a different site, we need to reinitialize
  // the trusted params for the new origin and restart the request.
  if (!original_origin.IsSameOriginWith(search_result_add_origin)) {
    request->site_for_cookies =
        net::SiteForCookies::FromOrigin(search_result_add_origin);
    request->request_initiator = search_result_add_origin;
    request->trusted_params = network::ResourceRequest::TrustedParams();
    request->trusted_params->isolation_info = net::IsolationInfo::Create(
        net::IsolationInfo::RequestType::kOther, search_result_add_origin,
        search_result_add_origin, net::SiteForCookies::FromOrigin(search_result_add_origin));
    VLOG(1) << request->trusted_params->isolation_info.site_for_cookies()
                   .ToDebugString();
  }
  //delegate_->RestartWithFlags(/* additional_load_flags */ 0);
}

}  // namespace brave_ads
