// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import OnboardingCreatePassword from './create-password/onboarding-create-password'
import { OnboardingWelcome } from './welcome'

export default {
  title: 'Wallet/Desktop/Onboarding'
}

export const _OnboardingWelcome = () => {
  return <OnboardingWelcome />
}

_OnboardingWelcome.story = {
  name: 'Welcome'
}

export const _OnboardingCreatePassword = () => {
  return <OnboardingCreatePassword />
}

_OnboardingCreatePassword.story = {
  name: 'Create Password'
}
