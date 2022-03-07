// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { getLocale } from '../../../common/locale'
import { BraveWallet, MarketDataTableColumnTypes, SortOrder } from '../../constants/types'
import { Table, Cell, Header, Row } from '../shared/datatable'
import {
  AssetsColumnItemSpacer,
  AssetsColumnWrapper,
  StyledWrapper,
  TableWrapper,
  TextWrapper
} from './style'
import {
  formatFiatAmountWithCommasAndDecimals,
  formatPricePercentageChange,
  formatPriceWithAbbreviation
} from '../../utils/format-prices'
import { AssetNameAndIcon } from '../asset-name-and-icon'
import { AssetPriceChange } from '../asset-price-change'
import useOnScreen from '../../common/hooks/on-screen'
import { CoinGeckoText } from '../desktop/views/portfolio/style'

export interface MarketDataHeader extends Header {
  id: MarketDataTableColumnTypes
}

export interface Props {
  headers: MarketDataHeader[]
  coinMarketData: BraveWallet.CoinMarket[]
  moreDataAvailable: boolean
  showEmptyState: boolean
  onShowMoreCoins: () => void
  onSort?: (column: MarketDataTableColumnTypes, newSortOrder: SortOrder) => void
}

const onScreenOptions = {
  rootMargin: '0px',
  threshold: 0
}

const renderCells = (coinMarkDataItem: BraveWallet.CoinMarket) => {
  const {
    name,
    symbol,
    image,
    currentPrice,
    priceChange24h,
    priceChangePercentage24h,
    marketCap,
    marketCapRank,
    totalVolume
  } = coinMarkDataItem

  const formattedPrice = formatFiatAmountWithCommasAndDecimals(currentPrice.toString(), 'USD')
  const formattedPercentageChange = formatPricePercentageChange(priceChangePercentage24h, 2, true)
  const formattedMarketCap = formatPriceWithAbbreviation(marketCap.toString(), 'USD', 1)
  const formattedVolume = formatPriceWithAbbreviation(totalVolume.toString(), 'USD', 1)
  const isDown = priceChange24h < 0

  const cellsContent: React.ReactNode[] = [
    <AssetsColumnWrapper>
      {/* Hidden until wishlist feature is available on the backend */}
      {/* <AssetsColumnItemSpacer>
          <AssetWishlistStar active={true} />
        </AssetsColumnItemSpacer> */}
      <AssetsColumnItemSpacer>
        <TextWrapper alignment="center">{marketCapRank}</TextWrapper>
      </AssetsColumnItemSpacer>
      <AssetNameAndIcon
        assetName={name}
        symbol={symbol}
        assetLogo={image}
      />
    </AssetsColumnWrapper>,

    // Price Column
    <TextWrapper alignment="right">{formattedPrice}</TextWrapper>,

    // Price Change Column
    <TextWrapper alignment="right">
      <AssetPriceChange
        isDown={isDown}
        priceChangePercentage={formattedPercentageChange}
      />
    </TextWrapper>,

    // Market Cap Column
    <TextWrapper alignment="right">{formattedMarketCap}</TextWrapper>,

    // Volume Column
    <TextWrapper alignment="right">{formattedVolume}</TextWrapper>

    // Line Chart Column
    // Commented out because priceHistory data is yet to be
    // available from the backend
    // <LineChartWrapper>
    //   <LineChart
    //     priceData={priceHistory}
    //     isLoading={false}
    //     isDisabled={false}
    //     isDown={isDown}
    //     isAsset={true}
    //     onUpdateBalance={() => {}}
    //     showPulsatingDot={false}
    //     showTooltip={false}
    //     customStyle={{
    //       height: '20px',
    //       width: '100%',
    //       marginBottom: '0px'
    //     }}
    //   />
    // </LineChartWrapper>
  ]

  const cells: Cell[] = cellsContent.map(cellContent => {
    return {
      content: cellContent
    }
  })

  return cells
}

export const MarketDataTable = (props: Props) => {
  const { headers, coinMarketData, moreDataAvailable, showEmptyState, onShowMoreCoins, onSort } = props
  const ref: any = React.useRef<HTMLDivElement>()
  const onScreen = useOnScreen<HTMLDivElement>(ref, onScreenOptions)

  React.useEffect(() => {
    if (onScreen && moreDataAvailable) {
      onShowMoreCoins()
    }
  }, [onScreen, moreDataAvailable])

  const rows: Row[] = React.useMemo(() => {
    const cells = coinMarketData.map((coinMarketItem: BraveWallet.CoinMarket) => {
      return renderCells(coinMarketItem)
    })

    return cells.map(cell => { return { content: cell } })
  }, [coinMarketData])

  return (
    <StyledWrapper>
      <TableWrapper>
        <Table
          headers={headers}
          rows={rows}
          onSort={onSort}
          stickyHeaders={true}
        >
          {/* Empty state message */}
          {showEmptyState && getLocale('braveWalletMarketDataNoAssetsFound')}
        </Table>
      </TableWrapper>
      {!moreDataAvailable && <CoinGeckoText>{getLocale('braveWalletPoweredByCoinGecko')}</CoinGeckoText>}
      <div ref={ref}/>
    </StyledWrapper>
  )
}
