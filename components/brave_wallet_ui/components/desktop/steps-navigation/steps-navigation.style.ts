// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import { Link, LinkProps } from 'react-router-dom'
import styled, { css } from 'styled-components'

const Font = css`
  font-family: 'Poppins';
  font-style: normal;
  font-weight: 600;
  font-size: 13px;
  line-height: 20px;
  color: ${(p) => p.theme.color.text02};
`

export const Wrapper = styled.div`
  display: flex;
  flex-direction: row;
  width: 100%;
  align-items: flex-start;
  justify-content: center;
`

export const BackButton = styled.button`
  ${Font}
  text-align: center;
  align-self: flex-start;
  color: ${(p) => p.theme.color.text01};
`

export const DotsWrapper = styled.div`
  display: flex;
  flex: 1;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  align-self: center;
  margin-right: 10%; /* compensate for the back-button */
`

export const Dot = styled(Link)<LinkProps & { isActive?: boolean }>`
  display: inline;
  width: 16px;
  height: 16px;
  border-radius: 25px;
  background-color: ${(p) => p.isActive ? p.theme.color.interactive05 : p.theme.color.divider01};
  margin-right: 12px;
`
