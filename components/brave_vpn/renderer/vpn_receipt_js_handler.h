/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_VPN_RENDERER_VPN_RECEIPT_JS_HANDLER_H_
#define BRAVE_COMPONENTS_BRAVE_VPN_RENDERER_VPN_RECEIPT_JS_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

//#include "brave/components/skus/common/skus_sdk.mojom.h"
#include "content/public/renderer/render_frame.h"
#include "content/public/renderer/render_frame_observer.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "url/gurl.h"
#include "v8/include/v8.h"

namespace brave_vpn {

// If present, this will inject a few methods (used by Brave accounts website)
// into window.chrome.braveVpn.*
//
// This is only intended to be used on account.brave.com and the dev / staging
// counterparts. The accounts website will use this if present which allows a
// safe way for the browser to intercept credentials which are used in the
// browser.
//
// This code in particular is meant to only be run on Android. This will expose
// a purchase token for the In-App-Purchase of Brave VPN if one exists.
//
// TODO(bsclifton): trying to cleanup the gin.
// Looking at chrome/renderer/net/net_error_page_controller.h as an example
class VpnReceiptJSHandler {
 public:
  explicit VpnReceiptJSHandler();
  VpnReceiptJSHandler(const VpnReceiptJSHandler&) = delete;
  VpnReceiptJSHandler& operator=(const VpnReceiptJSHandler&) = delete;
  ~VpnReceiptJSHandler();

  void AddJavaScriptObjectToFrame(v8::Local<v8::Context> context);

 private:
  void BindFunctionsToObject(v8::Isolate* isolate,
                             v8::Local<v8::Context> context);

  //content::RenderFrame* render_frame_;
  //mojo::Remote<skus::mojom::SkusService> skus_service_;
};

}  // namespace brave_vpn

#endif  // BRAVE_COMPONENTS_BRAVE_VPN_RENDERER_VPN_RECEIPT_JS_HANDLER_H_
