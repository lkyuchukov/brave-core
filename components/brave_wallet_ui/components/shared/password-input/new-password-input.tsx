import * as React from 'react'

// utils
import { getLocale } from '../../../../common/locale'

// hooks
import { usePasswordStrength } from '../../../common/hooks'

// style
import {
  StyledWrapper,
  InputWrapper,
  ToggleVisibilityButton,
  Input,
  ErrorText,
  ErrorRow,
  WarningIcon,
  InputLabel
} from './new-password-input.styles'

export interface NewPasswordValues {
  password: string
  isStrong: boolean
  confirmed: boolean
}

export interface Props {
  autoFocus?: boolean
  showToggleButton?: boolean
  onSubmit: (values: NewPasswordValues) => void
  onChange: (values: NewPasswordValues) => void
}

export const NewPasswordInput = ({
  autoFocus,
  showToggleButton,
  onSubmit
}: Props) => {
  // state
  const [showPassword, setShowPassword] = React.useState(false)

  // custom hooks
  const {
    confirmedPassword,
    hasConfirmedPasswordError,
    hasPasswordError,
    isStrongPassword,
    // isValid,
    onPasswordChanged,
    password,
    // passwordStrength,
    setConfirmedPassword
  } = usePasswordStrength()

  // memos
  const passwordError = React.useMemo(() => {
    return hasPasswordError
      ? getLocale('braveWalletCreatePasswordError')
      : ''
  }, [isStrongPassword, password])

  const passwordConfirmationError = React.useMemo(() => {
    return hasConfirmedPasswordError
      ? getLocale('braveWalletConfirmPasswordError')
      : ''
  }, [isStrongPassword, password, confirmedPassword])

  // methods
  const onTogglePasswordVisibility = () => {
    setShowPassword(prevShowPassword => !prevShowPassword)
  }

  const handlePasswordChanged = React.useCallback(async (event: React.ChangeEvent<HTMLInputElement>) => {
    onPasswordChanged(event.target.value)
  }, [onPasswordChanged])

  const handlePasswordConfirmationChanged = React.useCallback(async (event: React.ChangeEvent<HTMLInputElement>) => {
    setConfirmedPassword(event.target.value)
  }, [])

  const handleKeyDown = React.useCallback((event: React.KeyboardEvent<HTMLInputElement>) => {
    if (event.key === 'Enter') {
      if (!passwordConfirmationError) {
        onSubmit({
          confirmed: confirmedPassword === password,
          isStrong: isStrongPassword,
          password
        })
      }
    }
  }, [isStrongPassword, confirmedPassword, passwordConfirmationError, onSubmit])

  // render
  return (
    <>
    <StyledWrapper>
      <InputLabel>{getLocale('braveWalletCreatePasswordInput')}</InputLabel>
      <InputWrapper>
        <Input
          hasError={!!passwordError}
          type={(showToggleButton && showPassword) ? 'text' : 'password'}
          placeholder={getLocale('braveWalletCreatePasswordInput')}
          value={password}
          onChange={handlePasswordChanged}
          onKeyDown={handleKeyDown}
          autoFocus={autoFocus}
          autoComplete='off'
        />
        {showToggleButton &&
          <ToggleVisibilityButton
            showPassword={showPassword}
            onClick={onTogglePasswordVisibility}
          />
        }
      </InputWrapper>
      {!!passwordError &&
        <ErrorRow>
          <WarningIcon />
          <ErrorText>{passwordError}</ErrorText>
        </ErrorRow>
      }
    </StyledWrapper>

    <StyledWrapper>
      <InputLabel>{getLocale('braveWalletConfirmPasswordInput')}</InputLabel>
      <InputWrapper>
        <Input
          hasError={!!passwordConfirmationError}
          type={(showToggleButton && showPassword) ? 'text' : 'password'}
          placeholder={getLocale('braveWalletConfirmPasswordInput')}
          value={confirmedPassword}
          onChange={handlePasswordConfirmationChanged}
          onKeyDown={handleKeyDown}
          autoFocus={autoFocus}
          autoComplete='off'
        />
        {showToggleButton &&
          <ToggleVisibilityButton
            showPassword={showPassword}
            onClick={onTogglePasswordVisibility}
          />
        }
      </InputWrapper>
      {!!passwordConfirmationError &&
        <ErrorRow>
          <WarningIcon />
          <ErrorText>{passwordConfirmationError}</ErrorText>
        </ErrorRow>
      }
    </StyledWrapper>
    </>
  )
}

NewPasswordInput.defaultProps = {
  showToggleButton: true
}

export default NewPasswordInput
