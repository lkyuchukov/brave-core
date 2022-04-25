/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.util;

import org.chromium.base.Log;
import org.chromium.brave_wallet.mojom.BlockchainRegistry;
import org.chromium.brave_wallet.mojom.BlockchainToken;
import org.chromium.brave_wallet.mojom.BraveWalletConstants;
import org.chromium.brave_wallet.mojom.BraveWalletService;
import org.chromium.brave_wallet.mojom.CoinType;
import org.chromium.brave_wallet.mojom.OnRampProvider;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;

import java.lang.UnsupportedOperationException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class TokenUtils {
    // For  convenience, ERC20 also means ETH
    public enum TokenType {ERC20, ERC721, ALL};

    private static BlockchainToken[] filterTokens(BlockchainToken[] tokens, TokenType tokenType, boolean keepVisibleOnly) {
        ArrayList<BlockchainToken> arrayTokens = new ArrayList<>(Arrays.asList(tokens));
        Utils.removeIf(arrayTokens, t -> {
            boolean typeFilter;
            switch (tokenType) {
                case ERC20:
                    typeFilter = !t.isErc20 && !"ETH".equals(t.symbol);
                    break;
                case ERC721:
                    typeFilter = !t.isErc721;
                    break;
                case ALL:
                    typeFilter = false;
                    break;
                default:
                    throw new UnsupportedOperationException("Token type not supported.");
            }
            if (tokenType != TokenType.ALL)
            Log.w("TokenUtils", "filterTokens: token: name: " + t.symbol + " visible: " + String.valueOf(t.visible)
                + " chainId: " + t.chainId
                + " isErc20: " + String.valueOf(t.isErc20) + " isErc721: " + String.valueOf(t.isErc721)
                +  " typeFilter: " + String.valueOf(typeFilter) +  " keepVisibleOnly: " + String.valueOf(keepVisibleOnly)
                + " removed: " + String.valueOf(typeFilter || (keepVisibleOnly && !t.visible)));
            return typeFilter || (keepVisibleOnly && !t.visible);
        });

        return arrayTokens.toArray(new BlockchainToken[0]);
    }

    public static void getUserAssetsFiltered(BraveWalletService braveWalletService, String chainId,
            TokenType tokenType, BraveWalletService.GetUserAssets_Response callback) {
        braveWalletService.getUserAssets(chainId, CoinType.ETH, (BlockchainToken[] tokens) -> {
            BlockchainToken[] filteredTokens = filterTokens(tokens, tokenType, true);
            callback.call(filteredTokens);
        });
    }

    public static void getAllTokensFiltered(BraveWalletService braveWalletService,
            BlockchainRegistry blockchainRegistry, String chainId,
            TokenType tokenType, BlockchainRegistry.GetAllTokens_Response callback) {
        blockchainRegistry.getAllTokens(
                BraveWalletConstants.MAINNET_CHAIN_ID, CoinType.ETH, (BlockchainToken[] tokens) -> {
                    braveWalletService.getUserAssets(
                            chainId, CoinType.ETH, (BlockchainToken[] userTokens) -> {
                                BlockchainToken[] filteredTokens =
                                        filterTokens(concatenateTwoArrays(tokens, userTokens), tokenType, false);
                        callback.call(filteredTokens);
                    });
                });
    }

    public static void getBuyTokensFiltered(BlockchainRegistry blockchainRegistry,
            TokenType tokenType, BlockchainRegistry.GetAllTokens_Response callback) {
        blockchainRegistry.getBuyTokens(OnRampProvider.WYRE, BraveWalletConstants.MAINNET_CHAIN_ID,
                (BlockchainToken[] tokens) -> {
                    BlockchainToken[] filteredTokens = filterTokens(tokens, tokenType, false);
                    callback.call(filteredTokens);
                });
    }

    public static void isCustomToken(BlockchainToken token, BlockchainRegistry blockchainRegistry,
            org.chromium.mojo.bindings.Callbacks.Callback1<Boolean> callback) {
        blockchainRegistry.getAllTokens(
                BraveWalletConstants.MAINNET_CHAIN_ID, CoinType.ETH, (BlockchainToken[] tokens) -> {
                    boolean isCustom = true;
                    for (BlockchainToken tokenFromAll : tokens) {
                        if (token.contractAddress.equals(tokenFromAll.contractAddress)) {
                            isCustom = false;
                            break;
                        }
                    }
                    callback.call(isCustom);
                });
    }

    private static BlockchainToken[] concatenateTwoArrays(
            BlockchainToken[] arrayFirst, BlockchainToken[] arraySecond) {
        List<BlockchainToken> both = new ArrayList<>();

        Collections.addAll(both, arrayFirst);
        for (BlockchainToken tokenSecond : arraySecond) {
            boolean add = true;
            for (BlockchainToken tokenFirst : arrayFirst) {
                if (tokenFirst.contractAddress.equals(tokenSecond.contractAddress)) {
                    add = false;
                    break;
                }
            }
            if (add) {
                both.add(tokenSecond);
            }
        }

        return both.toArray(new BlockchainToken[both.size()]);
    }
}
