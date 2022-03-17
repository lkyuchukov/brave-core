// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef BRAVE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_BRAVE_NEW_TAB_SEARCH_HANDLER_H_
#define BRAVE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_BRAVE_NEW_TAB_SEARCH_HANDLER_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "brave/components/brave_new_tab_ui/brave_new_tab_searchbox.mojom.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace content {
class WebContents;
}  // namespace content

class Profile;

class BraveNewTabSearchHandler
    : public brave_new_tab_searchbox::mojom::PageHandler {
 public:
  BraveNewTabSearchHandler(
      mojo::PendingReceiver<brave_new_tab_searchbox::mojom::PageHandler>
          pending_page_handler,
      Profile* profile,
      content::WebContents* web_contents);
  ~BraveNewTabSearchHandler() override;

  BraveNewTabSearchHandler(const BraveNewTabSearchHandler&) = delete;
  BraveNewTabSearchHandler& operator=(const BraveNewTabSearchHandler&) = delete;

 private:
  // brave_new_tab_page::mojom::PageHandler
  void GoToBraveSearch(const std::string& input) override;

  mojo::Receiver<brave_new_tab_searchbox::mojom::PageHandler> page_handler_;
  raw_ptr<Profile> profile_ = nullptr;
  raw_ptr<content::WebContents> web_contents_ = nullptr;
};

#endif  // BRAVE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_BRAVE_NEW_TAB_SEARCH_HANDLER_H_
