/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// TODO: Validate that this is coming from the braveRewardsExtensionId
chrome.runtime.onConnect.addListener(function (port) {
  if (port.name === 'request-enable-rewards-panel') {
    let adsEnabled = false
    port.onMessage.addListener(function () {
      // Ignore any calls made after the first one
      if (!adsEnabled) {
        adsEnabled = true
        chrome.braveRewards.requestAdsEnabledPopupClosed(true)
      }
    })
    port.onDisconnect.addListener(function () {
      if (!adsEnabled) {
        chrome.braveRewards.requestAdsEnabledPopupClosed(false)
      }
    })
  }
})
