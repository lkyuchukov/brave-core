import * as React from 'react'
import { getLocale } from '../../../../common/locale'
// Styled Components
import {
  StyledWrapper,
  BackIcon
} from './style'

export interface Props {
  onSubmit: () => void
  showBorder?: boolean
}

const BackButton = (props: Props) => {
  const { onSubmit, showBorder } = props
  return (
    <StyledWrapper
      showBorder={showBorder}
      onClick={onSubmit}
    >
      <BackIcon />
      {getLocale('braveWalletBack')}
    </StyledWrapper>
  )
}
export default BackButton
