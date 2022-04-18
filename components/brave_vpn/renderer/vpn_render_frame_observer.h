// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef BRAVE_COMPONENTS_BRAVE_VPN_RENDERER_VPN_RENDER_FRAME_OBSERVER_H_
#define BRAVE_COMPONENTS_BRAVE_VPN_RENDERER_VPN_RENDER_FRAME_OBSERVER_H_

#include <memory>

#include "brave/components/brave_vpn/renderer/vpn_receipt_js_handler.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "v8/include/v8.h"

namespace brave_vpn {

// Used to conditionally inject a handler which exposes JavaScript methods
// to the Brave accounts website to expose VPN purchase token. The conditional
// logic ensures this only happens:
// - when VPN is enabled
// - on a Brave property (such as account.brave.com)
// - on Android
//
// Implementation-wise, those methods will only resolve in a regular
// (non-private / non-guest / non-Tor) context.
//
// See `renderer/brave_content_renderer_client.cc` for more information.
class VpnRenderFrameObserver : public content::RenderFrameObserver {
 public:
  explicit VpnRenderFrameObserver(content::RenderFrame* render_frame,
                                   int32_t world_id);
  VpnRenderFrameObserver(const VpnRenderFrameObserver&) = delete;
  VpnRenderFrameObserver& operator=(const VpnRenderFrameObserver&) = delete;
  ~VpnRenderFrameObserver() override;

  // RenderFrameObserver implementation.
  void DidCreateScriptContext(v8::Local<v8::Context> context,
                              int32_t world_id) override;

 private:
  // RenderFrameObserver implementation.
  void OnDestruct() override;

  bool IsAllowed();

  // Handle to "handler" JavaScript object functionality.
  std::unique_ptr<VpnReceiptJSHandler> native_javascript_handle_;
  int32_t world_id_;
};

}  // namespace brave_vpn

#endif  // BRAVE_COMPONENTS_BRAVE_VPN_RENDERER_VPN_RENDER_FRAME_OBSERVER_H_
