/* Copyright 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/renderer/search_result_ad_renderer_throttle.h"

#include "brave/components/brave_search/common/brave_search_utils.h"
#include "content/public/renderer/render_frame.h"
#include "services/network/public/cpp/resource_request.h"
#include "url/gurl.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/mojom/fetch/fetch_api_request.mojom-shared.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "third_party/blink/public/platform/web_security_origin.h"
#include "third_party/blink/public/platform/web_string.h"
#include "third_party/blink/public/platform/web_url.h"
#include "third_party/blink/public/platform/web_url_request.h"

namespace brave_ads {

namespace {

constexpr char kSearchAdsConfirmationHost[] = "search-ads-confirmation.brave.com";
constexpr char kSearchAdsConfirmationPath[] = "/v10/view";
constexpr char kCreativeInstanceIdParameterName[] = "creativeInstanceId";

std::string GetCreativeInstanceIdFromUrl(const GURL& url) {
  if (!url.is_valid() || !url.SchemeIs(url::kHttpsScheme) ||
      url.host_piece() != kSearchAdsConfirmationHost ||
      url.path_piece() != kSearchAdsConfirmationPath ||
      !url.has_query()) {
    return std::string();
  }

  base::StringPiece query_str = url.query_piece();
  url::Component query(0, static_cast<int>(query_str.length())), key, value;
  while (url::ExtractQueryKeyValue(query_str.data(), &query, &key, &value)) {
    base::StringPiece key_str = query_str.substr(key.begin, key.len);
    if (key_str == kCreativeInstanceIdParameterName) {
      base::StringPiece value_str = query_str.substr(value.begin, value.len);
      return static_cast<std::string>(value_str);
    }
  }

  return std::string();
}

}

std::unique_ptr<blink::URLLoaderThrottle>
SearchResultAdRendererThrottle::MaybeCreateThrottle(
    int render_frame_id, const blink::WebURLRequest& request) {
  if (request.GetRequestContext() != blink::mojom::RequestContextType::FETCH) {
    return nullptr;
  }

  absl::optional<blink::WebSecurityOrigin> top_frame_origin = request.TopFrameOrigin();
  if (!top_frame_origin) {
    return nullptr;
  }
  const GURL top_frame_origin_url = url::Origin(*top_frame_origin).GetURL();
  if (!brave_search::IsAllowedHost(top_frame_origin_url)) {
    return nullptr;
  }

  const GURL requestor_origin_url =
      url::Origin(request.RequestorOrigin()).GetURL();
  if (!brave_search::IsAllowedHost(requestor_origin_url)) {
    return nullptr;
  }

  const GURL url = static_cast<GURL>(request.Url());

  LOG(ERROR) << "**************************************************** " <<
      requestor_origin_url.spec();
  LOG(ERROR) << "**************************************************** " <<
      url.spec();

  std::string creative_instance_id = GetCreativeInstanceIdFromUrl(url);
  if (creative_instance_id.empty()) {
    return nullptr;
  }

  content::RenderFrame* render_frame =
      content::RenderFrame::FromRoutingID(render_frame_id);
  if (!render_frame || !render_frame->IsMainFrame()) {
    return nullptr;
  }

  mojo::PendingRemote<brave_ads::mojom::BraveAdsHost> brave_ads_pending_remote;
  render_frame->GetBrowserInterfaceBroker()->GetInterface(
      brave_ads_pending_remote.InitWithNewPipeAndPassReceiver());

  auto throttle = std::make_unique<SearchResultAdRendererThrottle>(
      std::move(brave_ads_pending_remote), std::move(creative_instance_id));

  return throttle;
}

SearchResultAdRendererThrottle::SearchResultAdRendererThrottle(
    mojo::PendingRemote<brave_ads::mojom::BraveAdsHost> brave_ads_pending_remote,
    std::string creative_instance_id)
  : brave_ads_pending_remote_(std::move(brave_ads_pending_remote)),
    creative_instance_id_(std::move(creative_instance_id)) {
  DCHECK(brave_ads_pending_remote_);
}

SearchResultAdRendererThrottle::~SearchResultAdRendererThrottle() = default;

void SearchResultAdRendererThrottle::DetachFromCurrentSequence() {}

void SearchResultAdRendererThrottle::WillStartRequest(
      network::ResourceRequest* request,
      bool* defer) {
  DCHECK_EQ(creative_instance_id_, GetCreativeInstanceIdFromUrl(request->url));
  DCHECK(brave_search::IsAllowedHost(request->request_initiator->GetURL()));
  DCHECK_EQ(request->resource_type,
            static_cast<int>(blink::mojom::ResourceType::kXhr));

  mojo::Remote<brave_ads::mojom::BraveAdsHost> brave_ads_remote(
        std::move(brave_ads_pending_remote_));
  DCHECK(brave_ads_remote.is_bound());
  brave_ads_remote.reset_on_disconnect();

  LOG(ERROR) << "**************************************************** " <<
      request->request_initiator->GetURL().spec();
  LOG(ERROR) << "**************************************************** " <<
      request->url.spec();

  brave_ads::mojom::BraveAdsHost* raw_brave_ads_remote = brave_ads_remote.get();
  raw_brave_ads_remote->MaybeTriggerAdViewConfirmation(
      creative_instance_id_, base::BindOnce(
          &SearchResultAdRendererThrottle::OnMaybeTriggerAdViewConfirmation,
          weak_factory_.GetWeakPtr(), std::move(brave_ads_remote)));

  *defer = true;
}

void SearchResultAdRendererThrottle::OnMaybeTriggerAdViewConfirmation(
    mojo::Remote<brave_ads::mojom::BraveAdsHost> brave_ads_remote,
    bool confirmation_triggered) {
  if (confirmation_triggered) {
    delegate_->CancelWithError(net::ERR_ABORTED);
  } else {
    delegate_->Resume();
  }
}

}  // namespace brave_ads
