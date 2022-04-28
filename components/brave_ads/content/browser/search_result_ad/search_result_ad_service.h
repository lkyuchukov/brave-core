/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_SERVICE_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_SERVICE_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "brave/vendor/bat-native-ads/include/bat/ads/public/interfaces/ads.mojom.h"
#include "components/keyed_service/core/keyed_service.h"
#include "content/public/browser/global_routing_id.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "third_party/blink/public/mojom/document_metadata/document_metadata.mojom.h"

namespace content {
class RenderFrameHost;
}

namespace brave_ads {

class AdsService;

using SearchResultAdsMap = std::map<std::string, ads::mojom::SearchResultAdPtr>;

class SearchResultAdService : public KeyedService {
 public:
  explicit SearchResultAdService(AdsService* ads_service);
  ~SearchResultAdService() override;

  SearchResultAdService(const SearchResultAdService&) = delete;
  SearchResultAdService& operator=(const SearchResultAdService&) = delete;

  void MaybeRetrieveSearchResultAd(content::RenderFrameHost* render_frame_host);

  void OnDidFinishNavigation(content::RenderFrameHost* render_frame_host);

  void OnWebContentsDestroyed(content::RenderFrameHost* render_frame_host);

  void MaybeTriggerSearchResultAdViewConfirmation(
    const std::string& creative_instance_id,
    content::GlobalRenderFrameHostId render_frame_host_id,
    base::OnceCallback<void(bool)> callback);

  void SetMetadataRequestFinishedCallbackForTesting(base::OnceClosure callback);

  AdsService* SetAdsServiceForTesting(AdsService* ads_service);

 private:
  struct ViewConfirmationCallbackInfo {
    ViewConfirmationCallbackInfo();
    ViewConfirmationCallbackInfo(ViewConfirmationCallbackInfo&& info);
    ViewConfirmationCallbackInfo& operator=(
        ViewConfirmationCallbackInfo&& info);
    ~ViewConfirmationCallbackInfo();

    std::string creative_instance_id;
    base::OnceCallback<void(bool)> callback;
  };

  void Reset(content::RenderFrameHost* render_frame_host);

  void OnRetrieveSearchResultAdEntities(
      mojo::Remote<blink::mojom::DocumentMetadata> document_metadata,
      content::GlobalRenderFrameHostId render_frame_host_id,
      blink::mojom::WebPagePtr web_page);

  void RunAdViewConfirmationPendingCallbacks(
      content::GlobalRenderFrameHostId render_frame_host_id,
      bool ad_was_triggered);

  bool TriggerAdViewConfirmation(
      const std::string& creative_instance_id,
      content::GlobalRenderFrameHostId render_frame_host_id);

  void OnTriggerSearchResultAdViewedEvents(
      bool success,
      const std::string& placement_id,
      ads::mojom::SearchResultAdEventType ad_event_type);

  raw_ptr<AdsService> ads_service_ = nullptr;

  std::map<content::GlobalRenderFrameHostId, SearchResultAdsMap>
      search_result_ads_map_;

  std::map<content::GlobalRenderFrameHostId,
           std::vector<ViewConfirmationCallbackInfo>>
               view_confirmation_pending_callbacks_;

  base::OnceClosure metadata_request_finished_callback_for_testing_;

  base::WeakPtrFactory<SearchResultAdService> weak_factory_{this};
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CONTENT_BROWSER_SEARCH_RESULT_AD_SEARCH_RESULT_AD_SERVICE_H_
