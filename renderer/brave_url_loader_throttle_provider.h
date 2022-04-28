// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BRAVE_RENDERER_BRAVE_URL_LOADER_THROTTLE_PROVIDER_H_
#define BRAVE_RENDERER_BRAVE_URL_LOADER_THROTTLE_PROVIDER_H_

#include <memory>
#include <vector>

#include "base/threading/thread_checker.h"
#include "components/safe_browsing/content/common/safe_browsing.mojom.h"
#include "extensions/buildflags/buildflags.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "third_party/blink/public/common/thread_safe_browser_interface_broker_proxy.h"
#include "third_party/blink/public/platform/url_loader_throttle_provider.h"

#include "chrome/renderer/url_loader_throttle_provider_impl.h"

class ChromeContentRendererClient;

class BraveURLLoaderThrottleProvider : public blink::URLLoaderThrottleProvider {
 public:
  BraveURLLoaderThrottleProvider(
      blink::ThreadSafeBrowserInterfaceBrokerProxy* broker,
      blink::URLLoaderThrottleProviderType type,
      ChromeContentRendererClient* chrome_content_renderer_client);

  BraveURLLoaderThrottleProvider(
      const BraveURLLoaderThrottleProvider& other) = delete;
  BraveURLLoaderThrottleProvider& operator=(
      const BraveURLLoaderThrottleProvider&) = delete;

  ~BraveURLLoaderThrottleProvider() override;

  // blink::URLLoaderThrottleProvider implementation.
  std::unique_ptr<blink::URLLoaderThrottleProvider> Clone() override;
  blink::WebVector<std::unique_ptr<blink::URLLoaderThrottle>> CreateThrottles(
      int render_frame_id,
      const blink::WebURLRequest& request) override;
  void SetOnline(bool is_online) override;

 private:
  BraveURLLoaderThrottleProvider();

  blink::URLLoaderThrottleProviderType provider_type_ =
      blink::URLLoaderThrottleProviderType::kFrame;

  std::unique_ptr<blink::URLLoaderThrottleProvider>
      url_loader_throttle_provider_impl_;
};

#endif  // BRAVE_RENDERER_BRAVE_URL_LOADER_THROTTLE_PROVIDER_H_
