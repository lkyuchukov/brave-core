/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <initializer_list>

#include "base/feature_list.h"
#include "brave/components/brave_shields/common/brave_shield_constants.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings.h"

namespace {

void FillRendererContentSettingsRules(
    const HostContentSettingsMap* map,
    std::initializer_list<
        std::pair<ContentSettingsType, ContentSettingsForOneType*>> settings) {
  for (const auto& setting : settings) {
    DCHECK(
        RendererContentSettingRules::IsRendererContentSetting(setting.first));
    map->GetSettingsForOneType(setting.first, setting.second);
  }
}

}  // namespace

#define BRAVE_GET_RENDER_CONTENT_SETTING_RULES                               \
  FillRendererContentSettingsRules(                                          \
      map,                                                                   \
      {                                                                      \
          {ContentSettingsType::AUTOPLAY, &rules->autoplay_rules},           \
          {ContentSettingsType::BRAVE_FINGERPRINTING_V2,                     \
           &rules->fingerprinting_rules},                                    \
          {ContentSettingsType::BRAVE_SHIELDS, &rules->brave_shields_rules}, \
          {ContentSettingsType::BRAVE_COSMETIC_FILTERING,                    \
           &rules->cosmetic_filtering_rules},                                \
      });

#include "src/components/content_settings/core/browser/content_settings_utils.cc"

#undef BRAVE_GET_RENDER_CONTENT_SETTING_RULES
