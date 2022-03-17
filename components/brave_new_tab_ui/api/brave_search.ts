/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as BraveNewTabSearch from 'gen/brave/components/brave_new_tab_ui/brave_new_tab_searchbox.mojom.m.js'

// Provide access to all the generated types
export * from 'gen/brave/components/brave_new_tab_ui/brave_new_tab_page.mojom.m.js'

interface API {
  pageHandler: BraveNewTabSearch.PageHandlerRemote
}

let ntpSearchAPIInstance: API

class NTPSearchAPI implements API {
  pageHandler = new BraveNewTabSearch.PageHandlerRemote()

  constructor () {
    const factory = BraveNewTabSearch.PageHandlerFactory.getRemote()
    factory.createPageHandler(this.pageHandler.$.bindNewPipeAndPassReceiver())
  }
}

export default function getNTPSearchAPI () {
  if (!ntpSearchAPIInstance) {
    ntpSearchAPIInstance = new NTPSearchAPI()
  }
  return ntpSearchAPIInstance
}
