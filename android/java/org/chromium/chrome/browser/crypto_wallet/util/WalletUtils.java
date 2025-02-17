/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.util;

import android.content.Context;

import org.chromium.brave_wallet.mojom.AccountInfo;
import org.chromium.brave_wallet.mojom.CoinType;
import org.chromium.chrome.R;

import java.util.ArrayList;
import java.util.List;

public class WalletUtils {
    public static String getUniqueNextAccountName(Context context, AccountInfo[] accountInfos,
            String cryptoAccountType, @CoinType.EnumType int coin) {
        List<AccountInfo> accountInfoList = new ArrayList<>();
        for (AccountInfo accountInfo : accountInfos) {
            if (accountInfo.coin == coin) {
                accountInfoList.add(accountInfo);
            }
        }
        return getUniqueNextAccountName(context, accountInfoList.toArray(new AccountInfo[0]),
                1 /* account name 1..n*/, cryptoAccountType);
    }

    private static String getUniqueNextAccountName(
            Context context, AccountInfo[] accountInfos, int number, String cryptoAccountTypeInfo) {
        String accountName = context.getString(R.string.new_account_prefix, cryptoAccountTypeInfo,
                String.valueOf(accountInfos.length + number));
        for (AccountInfo accountInfo : accountInfos) {
            if (accountInfo.name.equals(accountName)) {
                return getUniqueNextAccountName(
                        context, accountInfos, number + 1, cryptoAccountTypeInfo);
            }
        }
        return accountName;
    }
}
