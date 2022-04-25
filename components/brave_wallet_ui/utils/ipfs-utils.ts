// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

export const httpifyIpfsIconUrl = (url: string | undefined) => {
  if (url) {
    return `chrome://image/?${url.includes('ipfs://') ? url.replace('ipfs://', 'https://ipfs.io/ipfs/') : url}`
  }

  return ''
}
