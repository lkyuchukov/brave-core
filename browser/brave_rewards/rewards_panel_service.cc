/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_rewards/rewards_panel_service.h"

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"

namespace brave_rewards {

RewardsPanelService::RewardsPanelService(Profile* profile)
    : profile_(profile) {}

RewardsPanelService::~RewardsPanelService() = default;

bool RewardsPanelService::OpenRewardsPanel(const std::string& args) {
  LOG(ERROR) << "zenparsing OpenRewardsPanel: " << args;
  panel_args_ = args;
  bool handled = false;
  if (Browser* browser = chrome::FindTabbedBrowser(profile_, false)) {
    if (browser->window()->IsMinimized()) {
      browser->window()->Restore();
    }
    for (auto& observer : observers_) {
      if (observer.OnRewardsPanelRequested(browser)) {
        handled = true;
      }
    }
  }
  return handled;
}

void RewardsPanelService::AddObserver(Observer* observer) {
  observers_.AddObserver(observer);
}

void RewardsPanelService::RemoveObserver(Observer* observer) {
  observers_.RemoveObserver(observer);
}

std::string RewardsPanelService::TakePanelArguments() {
  return std::move(panel_args_);
}

}  // namespace brave_rewards
