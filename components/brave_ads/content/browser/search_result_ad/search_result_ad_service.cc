/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/content/browser/search_result_ad/search_result_ad_service.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "base/callback.h"
#include "base/containers/fixed_flat_set.h"
#include "base/containers/flat_set.h"
#include "base/feature_list.h"
#include "base/strings/string_number_conversions.h"
#include "brave/components/brave_ads/browser/ads_service.h"
#include "brave/components/brave_ads/common/features.h"
#include "brave/components/brave_search/common/brave_search_utils.h"
#include "content/public/browser/render_frame_host.h"
#include "services/service_manager/public/cpp/interface_provider.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

using ads::mojom::Conversion;
using ads::mojom::ConversionPtr;
using ads::mojom::SearchResultAd;
using ads::mojom::SearchResultAdPtr;

namespace brave_ads {

namespace {

constexpr char kSearchResultAdsType[] = "Product";
constexpr char kSearchResultAdType[] = "SearchResultAd";

constexpr char kContextPropertyName[] = "@context";
constexpr char kTypePropertyName[] = "@type";

constexpr char kDataPlacementId[] = "data-placement-id";
constexpr char kDataCreativeInstanceId[] = "data-creative-instance-id";
constexpr char kDataCreativeSetId[] = "data-creative-set-id";
constexpr char kDataCampaignId[] = "data-campaign-id";
constexpr char kDataAdvertiserId[] = "data-advertiser-id";
constexpr char kDataLandingPage[] = "data-landing-page";
constexpr char kDataHeadlineText[] = "data-headline-text";
constexpr char kDataDescription[] = "data-description";
constexpr char kDataRewardsValue[] = "data-rewards-value";
constexpr char kDataConversionTypeValue[] = "data-conversion-type-value";
constexpr char kDataConversionUrlPatternValue[] =
    "data-conversion-url-pattern-value";
constexpr char kDataConversionAdvertiserPublicKeyValue[] =
    "data-conversion-advertiser-public-key-value";
constexpr char kDataConversionObservationWindowValue[] =
    "data-conversion-observation-window-value";

constexpr auto kSearchResultAdAttributes =
    base::MakeFixedFlatSet<base::StringPiece>(
        {kDataPlacementId, kDataCreativeInstanceId, kDataCreativeSetId,
         kDataCampaignId, kDataAdvertiserId, kDataLandingPage,
         kDataHeadlineText, kDataDescription, kDataRewardsValue,
         kDataConversionTypeValue, kDataConversionUrlPatternValue,
         kDataConversionAdvertiserPublicKeyValue,
         kDataConversionObservationWindowValue});

bool GetStringValue(const schema_org::mojom::PropertyPtr& ad_property,
                    std::string* out_value) {
  DCHECK(ad_property);
  DCHECK(out_value);

  // Wrong attribute type
  if (!ad_property->values->is_string_values() ||
      ad_property->values->get_string_values().size() != 1) {
    return false;
  }

  *out_value = ad_property->values->get_string_values().front();

  return true;
}

bool GetIntValue(const schema_org::mojom::PropertyPtr& ad_property,
                 int32_t* out_value) {
  DCHECK(ad_property);
  DCHECK(out_value);

  // Wrong attribute type
  if (!ad_property->values->is_long_values() ||
      ad_property->values->get_long_values().size() != 1) {
    return false;
  }

  *out_value = ad_property->values->get_long_values().front();

  return true;
}

bool GetDoubleValue(const schema_org::mojom::PropertyPtr& ad_property,
                    double* out_value) {
  DCHECK(ad_property);
  DCHECK(out_value);

  // Wrong attribute type
  if (!ad_property->values->is_string_values() ||
      ad_property->values->get_string_values().size() != 1) {
    return false;
  }

  std::string value = ad_property->values->get_string_values().front();
  return base::StringToDouble(value, out_value);
}

bool SetSearchAdProperty(const schema_org::mojom::PropertyPtr& ad_property,
                         SearchResultAd* search_result_ad) {
  DCHECK(ad_property);
  DCHECK(search_result_ad);
  DCHECK(search_result_ad->conversion);

  const std::string& name = ad_property->name;
  if (name == kDataPlacementId) {
    return GetStringValue(ad_property, &search_result_ad->placement_id);
  } else if (name == kDataCreativeInstanceId) {
    return GetStringValue(ad_property, &search_result_ad->creative_instance_id);
  } else if (name == kDataCreativeSetId) {
    return GetStringValue(ad_property, &search_result_ad->creative_set_id);
  } else if (name == kDataCampaignId) {
    return GetStringValue(ad_property, &search_result_ad->campaign_id);
  } else if (name == kDataAdvertiserId) {
    return GetStringValue(ad_property, &search_result_ad->advertiser_id);
  } else if (name == kDataLandingPage) {
    return GetStringValue(ad_property, &search_result_ad->target_url);
  } else if (name == kDataHeadlineText) {
    return GetStringValue(ad_property, &search_result_ad->headline_text);
  } else if (name == kDataDescription) {
    return GetStringValue(ad_property, &search_result_ad->description);
  } else if (name == kDataRewardsValue) {
    return GetDoubleValue(ad_property, &search_result_ad->value);
  } else if (name == kDataConversionTypeValue) {
    return GetStringValue(ad_property, &search_result_ad->conversion->type);
  } else if (name == kDataConversionUrlPatternValue) {
    return GetStringValue(ad_property,
                          &search_result_ad->conversion->url_pattern);
  } else if (name == kDataConversionAdvertiserPublicKeyValue) {
    return GetStringValue(ad_property,
                          &search_result_ad->conversion->advertiser_public_key);
  } else if (name == kDataConversionObservationWindowValue) {
    return GetIntValue(ad_property,
                       &search_result_ad->conversion->observation_window);
  }

  NOTREACHED();

  return false;
}

absl::optional<SearchResultAdsMap> ParseSearchResultAdsMapEntityProperties(
    const schema_org::mojom::EntityPtr& entity) {
  DCHECK(entity);
  DCHECK_EQ(entity->type, kSearchResultAdsType);

  SearchResultAdsMap search_result_ads_list;

  for (const auto& property : entity->properties) {
    if (!property || property->name == kContextPropertyName ||
        property->name == kTypePropertyName) {
      continue;
    }

    // Search result ads list product could have only "@context" and "creatives"
    // properties.
    if (property->name != "creatives") {
      return absl::nullopt;
    }

    if (!property->values->is_entity_values() ||
        property->values->get_entity_values().empty()) {
      LOG(ERROR) << "Search result ad attributes list is empty";
      return SearchResultAdsMap();
    }

    for (const auto& ad_entity : property->values->get_entity_values()) {
      if (!ad_entity || ad_entity->type != kSearchResultAdType) {
        LOG(ERROR) << "Wrong search result ad type specified: "
                   << ad_entity->type;
        return SearchResultAdsMap();
      }

      if (property->name == kTypePropertyName) {
        continue;
      }

      SearchResultAdPtr search_result_ad = SearchResultAd::New();
      search_result_ad->conversion = Conversion::New();

      base::flat_set<base::StringPiece> found_attributes;
      for (const auto& ad_property : ad_entity->properties) {
        // Wrong attribute name
        const auto* it = std::find(kSearchResultAdAttributes.begin(),
                                   kSearchResultAdAttributes.end(),
                                   base::StringPiece(ad_property->name));

        if (it == kSearchResultAdAttributes.end()) {
          LOG(ERROR) << "Wrong search result ad attribute specified: "
                     << ad_property->name;
          return SearchResultAdsMap();
        }
        found_attributes.insert(*it);

        if (!SetSearchAdProperty(ad_property, search_result_ad.get())) {
          LOG(ERROR) << "Cannot read search result ad attribute value: "
                     << ad_property->name;
          return SearchResultAdsMap();
        }
      }

      // Not all of attributes were specified.
      if (found_attributes.size() != kSearchResultAdAttributes.size()) {
        std::vector<base::StringPiece> absent_attributes;
        std::set_difference(kSearchResultAdAttributes.begin(),
                            kSearchResultAdAttributes.end(),
                            found_attributes.begin(), found_attributes.end(),
                            std::back_inserter(absent_attributes));

        LOG(ERROR) << "Some of search result ad attributes were not specified: "
                   << base::JoinString(absent_attributes, ", ");

        return SearchResultAdsMap();
      }

      const std::string creative_instance_id = search_result_ad->creative_instance_id;
      search_result_ads_list.emplace(creative_instance_id,
                                     std::move(search_result_ad));
    }

    // Creatives has been parsed.
    break;
  }

  return search_result_ads_list;
}

void LogSearchResultAdsMap(const SearchResultAdsMap& search_result_ads_list) {
  if (!VLOG_IS_ON(1)) {
    return;
  }

  if (search_result_ads_list.empty()) {
    VLOG(1) << "Parsed search result ads list is empty.";
    return;
  }

  VLOG(1) << "Parsed search result ads list:";
  for (const auto& search_result_ad_pair : search_result_ads_list) {
    const auto& search_result_ad = search_result_ad_pair.second;
    VLOG(1) << "Ad with \"" << kDataPlacementId
            << "\": " << search_result_ad->placement_id;
    VLOG(1) << "  \"" << kDataCreativeInstanceId
            << "\": " << search_result_ad->creative_instance_id;
    VLOG(1) << "  \"" << kDataCreativeSetId
            << "\": " << search_result_ad->creative_set_id;
    VLOG(1) << "  \"" << kDataCampaignId
            << "\": " << search_result_ad->campaign_id;
    VLOG(1) << "  \"" << kDataAdvertiserId
            << "\": " << search_result_ad->advertiser_id;
    VLOG(1) << "  \"" << kDataLandingPage
            << "\": " << search_result_ad->target_url;
    VLOG(1) << "  \"" << kDataHeadlineText
            << "\": " << search_result_ad->headline_text;
    VLOG(1) << "  \"" << kDataDescription
            << "\": " << search_result_ad->description;
    VLOG(1) << "  \"" << kDataRewardsValue << "\": " << search_result_ad->value;
    VLOG(1) << "  \"" << kDataConversionTypeValue
            << "\": " << search_result_ad->conversion->type;
    VLOG(1) << "  \"" << kDataConversionUrlPatternValue
            << "\": " << search_result_ad->conversion->url_pattern;
    VLOG(1) << "  \"" << kDataConversionAdvertiserPublicKeyValue
            << "\": " << search_result_ad->conversion->advertiser_public_key;
    VLOG(1) << "  \"" << kDataConversionObservationWindowValue
            << "\": " << search_result_ad->conversion->observation_window;
  }
}

SearchResultAdsMap ParseWebPageEntities(blink::mojom::WebPagePtr web_page) {
  for (const auto& entity : web_page->entities) {
    if (entity->type != kSearchResultAdsType) {
      continue;
    }

    absl::optional<SearchResultAdsMap> search_result_ads_list =
        ParseSearchResultAdsMapEntityProperties(entity);

    if (search_result_ads_list) {
      LogSearchResultAdsMap(*search_result_ads_list);
      return std::move(*search_result_ads_list);
    }
  }

  VLOG(1) << "No search result ad found.";

  return SearchResultAdsMap();
}

}  // namespace

SearchResultAdService::ViewConfirmationCallbackInfo::ViewConfirmationCallbackInfo() = default;
SearchResultAdService::ViewConfirmationCallbackInfo::ViewConfirmationCallbackInfo(
    ViewConfirmationCallbackInfo&& info) = default;
SearchResultAdService::ViewConfirmationCallbackInfo& SearchResultAdService::ViewConfirmationCallbackInfo::operator=(
    ViewConfirmationCallbackInfo&& info) = default;
SearchResultAdService::ViewConfirmationCallbackInfo::~ViewConfirmationCallbackInfo() = default;

SearchResultAdService::SearchResultAdService(AdsService* ads_service)
    : ads_service_(ads_service) {}

SearchResultAdService::~SearchResultAdService() = default;

void SearchResultAdService::MaybeRetrieveSearchResultAd(
    content::RenderFrameHost* render_frame_host) {
  DCHECK(ads_service_);
  DCHECK(render_frame_host);

  content::GlobalRenderFrameHostId render_frame_host_id =
      render_frame_host->GetGlobalId();

  if (!ads_service_->IsEnabled() ||
      !base::FeatureList::IsEnabled(
          features::kSupportBraveSearchResultAdConfirmationEvents) ||
      !brave_search::IsAllowedHost(render_frame_host->GetLastCommittedURL())) {
    if (metadata_request_finished_callback_for_testing_) {
      std::move(metadata_request_finished_callback_for_testing_).Run();
    }
    search_result_ads_map_[render_frame_host_id] = SearchResultAdsMap();
    RunAdViewConfirmationPendingCallbacks(render_frame_host_id, false);
    return;
  }

  mojo::Remote<blink::mojom::DocumentMetadata> document_metadata;
  render_frame_host->GetRemoteInterfaces()->GetInterface(
      document_metadata.BindNewPipeAndPassReceiver());
  DCHECK(document_metadata.is_bound());
  document_metadata.reset_on_disconnect();

  blink::mojom::DocumentMetadata* raw_document_metadata =
      document_metadata.get();
  raw_document_metadata->GetEntities(
      base::BindOnce(&SearchResultAdService::OnRetrieveSearchResultAdEntities,
                     weak_factory_.GetWeakPtr(), std::move(document_metadata),
                     render_frame_host_id));
}

void SearchResultAdService::OnDidFinishNavigation(
    content::RenderFrameHost* render_frame_host) {
  Reset(render_frame_host);
}

void SearchResultAdService::OnWebContentsDestroyed(
    content::RenderFrameHost* render_frame_host) {
  Reset(render_frame_host);
}

void SearchResultAdService::MaybeTriggerSearchResultAdViewConfirmation(
    const std::string& creative_instance_id,
    content::GlobalRenderFrameHostId render_frame_host_id,
    base::OnceCallback<void(bool)> callback) {
  if (!ads_service_ || !ads_service_->IsEnabled()) {
    std::move(callback).Run(false);
    return;
  }
  LOG(ERROR) << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

  LOG(ERROR) << render_frame_host_id;

  auto ads_map_it = search_result_ads_map_.find(render_frame_host_id);
  if (ads_map_it == search_result_ads_map_.end()) {
    ViewConfirmationCallbackInfo callback_info;
    callback_info.creative_instance_id = creative_instance_id;
    callback_info.callback = std::move(callback);
    view_confirmation_pending_callbacks_[render_frame_host_id].push_back(
        std::move(callback_info));
    return;
  }
  LOG(ERROR) << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

  auto ad_it = ads_map_it->second.find(creative_instance_id);
  if (ad_it == ads_map_it->second.end()) {
    std::move(callback).Run(false);
    return;
  }
  LOG(ERROR) << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

  bool was_triggered = TriggerAdViewConfirmation(creative_instance_id,
                                                 render_frame_host_id);
  std::move(callback).Run(was_triggered);
}

void SearchResultAdService::SetMetadataRequestFinishedCallbackForTesting(
    base::OnceClosure callback) {
  metadata_request_finished_callback_for_testing_ = std::move(callback);
}

AdsService* SearchResultAdService::SetAdsServiceForTesting(
    AdsService* ads_service) {
  AdsService* previous_ads_service = ads_service_.get();
  ads_service_ = ads_service;
  return previous_ads_service;
}

void SearchResultAdService::Reset(content::RenderFrameHost* render_frame_host) {
  if (!render_frame_host) {
    return;
  }

  content::GlobalRenderFrameHostId render_frame_host_id =
      render_frame_host->GetGlobalId();
  LOG(ERROR) << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
  LOG(ERROR) << render_frame_host_id;

  view_confirmation_pending_callbacks_.erase(render_frame_host_id);
  search_result_ads_map_.erase(render_frame_host_id);
}

void SearchResultAdService::OnRetrieveSearchResultAdEntities(
    mojo::Remote<blink::mojom::DocumentMetadata> document_metadata,
    content::GlobalRenderFrameHostId render_frame_host_id,
    blink::mojom::WebPagePtr web_page) {
  if (metadata_request_finished_callback_for_testing_) {
    std::move(metadata_request_finished_callback_for_testing_).Run();
  }

  if (!web_page) {
    search_result_ads_map_[render_frame_host_id] = SearchResultAdsMap();
    RunAdViewConfirmationPendingCallbacks(render_frame_host_id, false);
    return;
  }

  LOG(ERROR) << "################################################";

  LOG(ERROR) << render_frame_host_id;

  SearchResultAdsMap search_result_ads =
      ParseWebPageEntities(std::move(web_page));

  search_result_ads_map_.emplace(render_frame_host_id,
                                 std::move(search_result_ads));

  RunAdViewConfirmationPendingCallbacks(render_frame_host_id, true);
}

void SearchResultAdService::RunAdViewConfirmationPendingCallbacks(
    content::GlobalRenderFrameHostId render_frame_host_id,
    bool ads_fetched) {
  for (auto& callback_info : view_confirmation_pending_callbacks_[render_frame_host_id]) {
    bool ad_was_triggered = false;
    if (ads_fetched) {
      ad_was_triggered = TriggerAdViewConfirmation(
          callback_info.creative_instance_id,
          render_frame_host_id);
    }
    std::move(callback_info.callback).Run(ad_was_triggered);
  }
  view_confirmation_pending_callbacks_.erase(render_frame_host_id);
}

bool SearchResultAdService::TriggerAdViewConfirmation(
    const std::string& creative_instance_id,
    content::GlobalRenderFrameHostId render_frame_host_id) {
  DCHECK(ads_service_);

  SearchResultAdsMap& ads_map = search_result_ads_map_[render_frame_host_id];
  auto it = ads_map.find(creative_instance_id);
  if (it == ads_map.end()) {
    return false;
  }

  ads::mojom::SearchResultAdPtr search_result_ad = std::move(it->second);
  ads_service_->TriggerSearchResultAdEvent(
      std::move(search_result_ad), ads::mojom::SearchResultAdEventType::kViewed,
      base::BindOnce(
          &SearchResultAdService::OnTriggerSearchResultAdViewedEvents,
          weak_factory_.GetWeakPtr()));

  ads_map.erase(creative_instance_id);

  return true;
}

void SearchResultAdService::OnTriggerSearchResultAdViewedEvents(
    const bool success,
    const std::string& placement_id,
    ads::mojom::SearchResultAdEventType ad_event_type) {
  DCHECK_EQ(ad_event_type, ads::mojom::SearchResultAdEventType::kViewed);
  if (!success) {
    VLOG(1) << "Failed to trigger search result ad viewed event for "
            << placement_id;
  }
}

}  // namespace brave_ads
