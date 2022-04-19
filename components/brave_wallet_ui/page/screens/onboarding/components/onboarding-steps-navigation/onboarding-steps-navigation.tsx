// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

/* eslint-disable @typescript-eslint/array-type */
import * as React from 'react'
import { useHistory } from 'react-router'

// routes
import { WalletRoutes } from '../../../../../constants/types'

// components
import StepsNavigation, { StepsNavigationProps } from '../../../../../components/desktop/steps-navigation/steps-navigation'

export enum OnboardingSteps {
  welcome = 'welcome',
  createPassword = 'create-password',
  viewRecoveryPhrase = 'view-recovery-phrase',
  backupRecoveryPhrase = 'backup-recovery-phrase',
  verifyRecoveryPhrase = 'verify-recovery-phrase',
  complete = 'complete'
}

const STEPS: OnboardingSteps[] = [
  OnboardingSteps.welcome,
  OnboardingSteps.createPassword,
  OnboardingSteps.viewRecoveryPhrase,
  OnboardingSteps.backupRecoveryPhrase,
  OnboardingSteps.verifyRecoveryPhrase,
  OnboardingSteps.complete
]

interface Props extends Omit<StepsNavigationProps<OnboardingSteps>, 'goBack' | 'steps'> {}

export const OnboardingStepsNavigation = ({ currentStep }: Props) => {
  // routing
  const history = useHistory()

  const goBack = React.useCallback(() => {
    history.push(WalletRoutes.Onboarding)
  }, [])

  return <StepsNavigation
    goBack={goBack}
    steps={STEPS}
    currentStep={currentStep}
  />
}

export default OnboardingStepsNavigation
