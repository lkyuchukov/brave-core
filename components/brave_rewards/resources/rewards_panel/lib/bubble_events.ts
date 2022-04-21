/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

interface Listener {
  onBubbleOpening: () => void
  onBubbleClosing: () => void
}

type State = 'open' | 'closed'

let state: State = document.visibilityState === 'hidden' ? 'closed' : 'open'
const listeners: Array<Listener> = []

document.addEventListener('visibilitychange', () => {
  if (document.visibilityState === 'hidden') {
    if (state === 'open') {
      state = 'closed'
      for (const listener of listeners) {
        listener.onBubbleClosing()
      }
    } else {
      state = 'open'
      for (const listener of listeners) {
        listener.onBubbleOpening()
      }
    }
  }
})

export function addBubbleListener (listener: Listener) {
  listeners.push(listener)
}
