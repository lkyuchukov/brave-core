/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_BRAVE_REWARDS_REWARDS_PANEL_SERVICE_H_
#define BRAVE_BROWSER_BRAVE_REWARDS_REWARDS_PANEL_SERVICE_H_

#include "base/memory/raw_ptr.h"
#include "base/observer_list.h"
#include "base/scoped_observation.h"
#include "components/keyed_service/core/keyed_service.h"

class Browser;
class Profile;

namespace brave_rewards {

class RewardsPanelService : public KeyedService {
 public:
  explicit RewardsPanelService(Profile* profile);
  ~RewardsPanelService() override;

  bool OpenRewardsPanel(const std::string& args);

  class Observer : public base::CheckedObserver {
   public:
    virtual bool OnRewardsPanelRequested(Browser* browser) = 0;
  };

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  using Observation = base::ScopedObservation<RewardsPanelService, Observer>;

  std::string TakePanelArguments();

 private:
  raw_ptr<Profile> profile_ = nullptr;
  base::ObserverList<Observer> observers_;
  std::string panel_args_;
};

}  // namespace brave_rewards

#endif  // BRAVE_BROWSER_BRAVE_REWARDS_REWARDS_PANEL_SERVICE_H_
