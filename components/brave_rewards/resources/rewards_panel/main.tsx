/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import * as ReactDOM from 'react-dom'

import { App } from './components/app'
import { createHost } from './lib/extension_host'

function BubbleRoot () {
  const [host] = React.useState(createHost())
  const [visibility, setVisibility] = React.useState(document.visibilityState)

  React.useEffect(() => {
    chrome.send('pageReady')
    const listener = () => { setVisibility(document.visibilityState) }
    document.addEventListener('visibilitychange', listener)
    return () => { document.removeEventListener('visibilitychange', listener) }
  }, [])

  React.useEffect(() => {
    // NOTE: When the bubble manager displays a WebUI bubble from a cached web
    // contents, the visibility state of the document will transition to
    // "visible" and then immediately to "hidden". We must notify C++ when we
    // are ready to show the content to the user. At that point the visibility
    // will transition back to "visible".
    if (visibility === 'visible') {
      chrome.send('showUI')
    }
  }, [visibility])

  return <App host={host} />
}

function onReady () {
  ReactDOM.render(<BubbleRoot />, document.getElementById('root'))
}

if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', onReady)
} else {
  onReady()
}
