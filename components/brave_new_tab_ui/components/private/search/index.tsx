import * as React from 'react'
import styled from 'styled-components'
import getNTPSearchAPI from '../../../api/brave_search'

const Form = styled.form`
  --focus-color: transparent;
  font-family: ${p => p.theme.fontFamily.heading};
  color: white;
  font-size: 18px;
  font-weight: 400;
  background-color: black;
  border-radius: 8px;
  padding: 5px 16px;
  border: 4px solid var(--focus-color);

  &:focus-within {
    --focus-color: ${p => p.theme.color.focusBorder}
  }

  input[type="text"] {
    width: 100%;
    height: 36px;
    border: 0;
    background-color: black;

    &:focus {
      outline: 0;
    }

    &::placeholder {
      color: ${p => p.theme.color.text03};
    }
  }
`

function Search () {
  const [value, setValue] = React.useState('')

  const onInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setValue(e.currentTarget.value)
  }

  const handleSubmit = (e: React.ChangeEvent<HTMLFormElement>) => {
    e.preventDefault()
    getNTPSearchAPI().pageHandler.goToBraveSearch(value)
  }

  return (
    <Form onSubmit={handleSubmit}>
      <input onChange={onInputChange} type="text" placeholder="Search the web privately" value={value} autoCapitalize="off" autoComplete="off" autoCorrect="off" spellCheck="false" aria-label="Search" title="Search" aria-autocomplete="none" aria-haspopup="false" maxLength={2048} />
    </Form>
  )
}

export default Search
