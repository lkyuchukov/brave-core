// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import BigNumber from 'bignumber.js'

import { CurrencySymbols } from './currency-symbols'

export const formatWithCommasAndDecimals = (value: string, decimals?: number) => {
  // empty string indicates unknown balance
  if (value === '') {
    return ''
  }

  const valueBN = new BigNumber(value)

  // We sometimes return Unlimited as a value
  if (valueBN.isNaN()) {
    return value
  }

  if (valueBN.isZero()) {
    return '0.00'
  }

  const fmt = {
    decimalSeparator: '.',
    groupSeparator: ',',
    groupSize: 3
  } as BigNumber.Format

  if (decimals && valueBN.isGreaterThan(10 ** -decimals)) {
    return valueBN.toFormat(decimals, BigNumber.ROUND_UP, fmt)
  }

  if (valueBN.isGreaterThanOrEqualTo(10)) {
    return valueBN.toFormat(2, BigNumber.ROUND_UP, fmt)
  }

  if (valueBN.isGreaterThanOrEqualTo(1)) {
    return valueBN.toFormat(3, BigNumber.ROUND_UP, fmt)
  }

  return valueBN.toFormat(fmt)
}

export const formatFiatAmountWithCommasAndDecimals = (value: string, defaultCurrency: string): string => {
  if (!value) {
    return ''
  }

  const currencySymbol = CurrencySymbols[defaultCurrency]

  // Check to make sure a formatted value is returned before showing the fiat symbol
  if (!formatWithCommasAndDecimals(value, 2)) {
    return ''
  }
  return currencySymbol + formatWithCommasAndDecimals(value, 2)
}

export const formatTokenAmountWithCommasAndDecimals = (value: string, symbol: string): string => {
  // Empty string indicates unknown balance
  if (!value && !symbol) {
    return ''
  }
  // Check to make sure a formatted value is returned before showing the symbol
  if (!formatWithCommasAndDecimals(value)) {
    return ''
  }
  return formatWithCommasAndDecimals(value) + ' ' + symbol
}

export const formatPriceWithAbbreviation = (value: string, defaultCurrency: string, decimals: number = 2): string => {
  if (!value || isNaN(Number(value))) {
    return ''
  }
  const currencySymbol = CurrencySymbols[defaultCurrency]
  const number = Number(value)

  const min = 1e3 // 1000

  if (number >= min) {
    const units = ['k', 'M', 'B', 'T']
    const order = Math.floor(Math.log(number) / Math.log(1000))
    const unitName = units[order - 1]
    const fixedPointNumber = Number((number / 1000 ** order).toFixed(decimals))
    const formattedNumber = new Intl.NumberFormat(navigator.language).format(fixedPointNumber)
    return currencySymbol + formattedNumber + unitName
  }

  return number.toFixed(decimals).toString()
}

export const formatPricePercentageChange = (value: number, decimals: number, absoluteValue = true): string => {
  if (!value) {
    return ''
  }

  const formattedValue = new Intl.NumberFormat(navigator.language, {
    minimumFractionDigits: decimals,
    maximumFractionDigits: decimals
  }).format(value)

  if (absoluteValue && formattedValue.startsWith('-')) {
    return formattedValue.substring(1) + '%' // remove the '-' sign
  }

  return formattedValue + '%'
}

export const abbreviateNumber = (number: number, decimals: number = 2): string => {
  const min = 1e3 // 1000

  if (isNaN(number)) {
    return number.toString()
  }

  if (number >= min) {
    const units = ['k', 'M', 'B', 'T']
    const order = Math.floor(Math.log(number) / Math.log(1000))
    const unitName = units[order - 1]
    const num = Number((number / 1000 ** order).toFixed(decimals))

    return num + unitName
  }

  return number.toFixed(decimals).toString()
}
