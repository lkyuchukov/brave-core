// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { BackButton } from '../../shared'

import { Dot, DotsWrapper, Wrapper } from './steps-navigation.style'

export interface StepsNavigationProps<T extends string> {
  readonly steps: T[]
  currentStep: T
  goBack: () => void
}

export const StepsNavigation: <T extends string>(
  props: StepsNavigationProps<T>
) => JSX.Element = ({
  currentStep,
  goBack,
  steps
}) => {
  return (
    <Wrapper>

      <BackButton
        showBorder={false}
        onSubmit={goBack}
      />

      <DotsWrapper>
        {steps.map((stepName) => {
          return <Dot
          to={`/${stepName}`}
          key={stepName}
          isActive={currentStep === stepName}
          />
        })}
      </DotsWrapper>

    </Wrapper>
  )
}

export default StepsNavigation
