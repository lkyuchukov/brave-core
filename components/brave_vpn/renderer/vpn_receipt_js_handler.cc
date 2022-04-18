/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_vpn/renderer/vpn_receipt_js_handler.h"

#include <tuple>
#include <utility>

#include "base/base64.h"
#include "base/json/json_reader.h"
#include "base/no_destructor.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/renderer/v8_value_converter.h"
#include "gin/arguments.h"
#include "gin/function_template.h"
#include "third_party/blink/public/common/browser_interface_broker_proxy.h"
#include "third_party/blink/public/mojom/devtools/console_message.mojom.h"
#include "third_party/blink/public/platform/web_string.h"
#include "third_party/blink/public/web/blink.h"
#include "third_party/blink/public/web/web_console_message.h"
#include "third_party/blink/public/web/web_local_frame.h"
#include "third_party/blink/public/web/web_script_source.h"

namespace {
std::string FormatReceipt(std::string purchase_token) {
  std::string type = "android";
  std::string response =
      base::StringPrintf("{\"type\": \"%s\", \"raw_receipt\": \"%s\"}",
                         type.c_str(), purchase_token.c_str());
  base::Base64Encode(response, &response);
  return response;
}
}  // namespace

namespace brave_vpn {

VpnReceiptJSHandler::VpnReceiptJSHandler() {}

VpnReceiptJSHandler::~VpnReceiptJSHandler() = default;

void VpnReceiptJSHandler::AddJavaScriptObjectToFrame(
    v8::Local<v8::Context> context) {
  v8::Isolate* isolate = blink::MainThreadIsolate();
  v8::HandleScope handle_scope(isolate);
  if (context.IsEmpty())
    return;

  v8::Context::Scope context_scope(context);

  BindFunctionsToObject(isolate, context);
}

void VpnReceiptJSHandler::BindFunctionsToObject(
    v8::Isolate* isolate,
    v8::Local<v8::Context> context) {
  v8::Local<v8::Object> global = context->Global();

  // window.chrome
  v8::Local<v8::Object> chrome_obj;
  v8::Local<v8::Value> chrome_value;
  if (!global->Get(context, gin::StringToV8(isolate, "chrome"))
           .ToLocal(&chrome_value) ||
      !chrome_value->IsObject()) {
    chrome_obj = v8::Object::New(isolate);
    global->Set(context, gin::StringToSymbol(isolate, "chrome"), chrome_obj)
        .Check();
  } else {
    chrome_obj = chrome_value->ToObject(context).ToLocalChecked();
  }

  // window.chrome.braveVpn
  v8::Local<v8::Object> vpn_obj;
  v8::Local<v8::Value> vpn_value;
  if (!chrome_obj->Get(context, gin::StringToV8(isolate, "braveVpn"))
           .ToLocal(&vpn_value) ||
      !vpn_value->IsObject()) {
    vpn_obj = v8::Object::New(isolate);
    chrome_obj->Set(context, gin::StringToSymbol(isolate, "braveVpn"), vpn_obj)
        .Check();
  } else {
    vpn_obj = vpn_value->ToObject(context).ToLocalChecked();
  }

  // window.chrome.braveVpn.receipt
  std::string purchase_token =
      "oohdhbmbebmciddpbcicgnko.AO-J1OxJGS6-"
      "tNYvzofx7RO2hJSEgQmi6tOrLHEB4zJ2OhsyhX3mhEe4QKS0MVxtJCBNIAlBP5jAgDPqdXDN"
      "z15JhIXt5QYcIExIxe5H5ifbhAsHILlUXlE";
  // TODO(bsclifton): get actual purchase token from device
  vpn_obj
      ->Set(context, gin::StringToSymbol(isolate, "receipt"),
            gin::StringToV8(isolate, FormatReceipt(purchase_token)))
      .Check();
}

}  // namespace brave_vpn
