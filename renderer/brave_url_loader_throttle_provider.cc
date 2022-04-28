// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "brave/renderer/brave_url_loader_throttle_provider.h"

#include <utility>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/feature_list.h"
#include "base/memory/ptr_util.h"
#include "base/threading/thread_task_runner_handle.h"
#include "build/build_config.h"
#include "build/chromeos_buildflags.h"
#include "chrome/common/google_url_loader_throttle.h"
#include "chrome/renderer/chrome_content_renderer_client.h"
#include "chrome/renderer/chrome_render_frame_observer.h"
#include "chrome/renderer/chrome_render_thread_observer.h"
#include "components/no_state_prefetch/renderer/no_state_prefetch_helper.h"
#include "components/safe_browsing/content/renderer/renderer_url_loader_throttle.h"
#include "components/safe_browsing/core/common/features.h"
#include "content/public/common/content_features.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_thread.h"
#include "content/public/renderer/render_view.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/common/loader/resource_type_util.h"
#include "third_party/blink/public/common/thread_safe_browser_interface_broker_proxy.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"


#include "base/memory/ptr_util.h"
#include "brave/components/brave_ads/renderer/search_result_ad_renderer_throttle.h"


BraveURLLoaderThrottleProvider::BraveURLLoaderThrottleProvider(
    blink::ThreadSafeBrowserInterfaceBrokerProxy* broker,
    blink::URLLoaderThrottleProviderType type,
    ChromeContentRendererClient* chrome_content_renderer_client)
    : provider_type_(type),
      url_loader_throttle_provider_impl_(
          std::make_unique<URLLoaderThrottleProviderImpl>(
              broker, type, chrome_content_renderer_client)) {
  LOG(ERROR) << "#####################################################";
}

BraveURLLoaderThrottleProvider::~BraveURLLoaderThrottleProvider() {
  LOG(ERROR) << "#####################################################";
}

BraveURLLoaderThrottleProvider::BraveURLLoaderThrottleProvider() {}

std::unique_ptr<blink::URLLoaderThrottleProvider>
BraveURLLoaderThrottleProvider::Clone() {
  auto throttle_provider_clone =
      base::WrapUnique(new BraveURLLoaderThrottleProvider());

  throttle_provider_clone->provider_type_ = provider_type_;
  throttle_provider_clone->url_loader_throttle_provider_impl_ =
      url_loader_throttle_provider_impl_->Clone();
  return throttle_provider_clone;
}

blink::WebVector<std::unique_ptr<blink::URLLoaderThrottle>>
BraveURLLoaderThrottleProvider::CreateThrottles(
    int render_frame_id,
    const blink::WebURLRequest& request) {
  blink::WebVector<std::unique_ptr<blink::URLLoaderThrottle>> throttles =
      url_loader_throttle_provider_impl_->CreateThrottles(render_frame_id, request);

  // const network::mojom::RequestDestination request_destination =
  //     request.GetRequestDestination();

  // Some throttles have already been added in the browser for frame resources.
  // Don't add them for frame requests.
  // bool is_frame_resource =
  //     blink::IsRequestDestinationFrame(request_destination);

  if (provider_type_ == blink::URLLoaderThrottleProviderType::kFrame) {
    auto search_result_ad_throttle =
        brave_ads::SearchResultAdRendererThrottle::MaybeCreateThrottle(
            render_frame_id, request);
    if (search_result_ad_throttle) {
      throttles.emplace_back(std::move(search_result_ad_throttle));
    }
  }

  return throttles;
}

void BraveURLLoaderThrottleProvider::SetOnline(bool is_online) {
  url_loader_throttle_provider_impl_->SetOnline(is_online);
}
