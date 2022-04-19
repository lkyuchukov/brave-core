// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

import {
  StyledWrapper,
  Title,
  Description,
  NextButtonRow
} from './onboarding-create-password.style'

// utils
import { getLocale } from '../../../../../common/locale'

// components
import { WalletPageLayout } from '../../../../components/desktop'
import { NavButton } from '../../../../components/extension'
import { NewPasswordInput, NewPasswordValues } from '../../../../components/shared/password-input/new-password-input'
import { OnboardingSteps, OnboardingStepsNavigation } from '../components/onboarding-steps-navigation/onboarding-steps-navigation'

export const OnboardingCreatePassword = () => {
  // state
  const [newPassword, setNewPassword] = React.useState('')
  const [newPasswordHasErrors, setNewPasswordHasErrors] = React.useState(false)

  // methods
  const handlePasswordChange = React.useCallback(({ confirmed, isStrong, password }: NewPasswordValues) => {
    alert('done')
    setNewPasswordHasErrors(!confirmed && !isStrong)
    setNewPassword(password)
  }, [])

  const nextStep = React.useCallback(() => {
    alert('next')
    alert(newPassword)
  }, [newPassword])

  const handlePasswordSubmit = React.useCallback(({ confirmed, isStrong, password }: NewPasswordValues) => {
    setNewPassword(password)
    setNewPasswordHasErrors(!confirmed && !isStrong)
    alert(password)
    if (confirmed && isStrong) {
      nextStep()
    }
  }, [nextStep])

  // render
  return (
    <WalletPageLayout>
      <StyledWrapper>

        <OnboardingStepsNavigation currentStep={OnboardingSteps.createPassword} />

        <Title>{getLocale('braveWalletCreatePasswordTitle')}</Title>
        <Description>{getLocale('braveWalletCreatePasswordDescription')}</Description>

        <NewPasswordInput
          autoFocus={true}
          onSubmit={handlePasswordSubmit}
          onChange={handlePasswordChange}
        />

        <NextButtonRow>
          <NavButton
            buttonType='primary'
            text={getLocale('braveWalletButtonContinue')}
            onSubmit={nextStep}
            disabled={newPasswordHasErrors}
          />
        </NextButtonRow>

      </StyledWrapper>
    </WalletPageLayout>
  )
}

export default OnboardingCreatePassword
