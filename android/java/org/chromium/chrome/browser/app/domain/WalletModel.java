/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.app.domain;

import android.content.Context;

import org.chromium.brave_wallet.mojom.AssetRatioService;
import org.chromium.brave_wallet.mojom.BlockchainRegistry;
import org.chromium.brave_wallet.mojom.BraveWalletService;
import org.chromium.brave_wallet.mojom.CoinType;
import org.chromium.brave_wallet.mojom.EthTxManagerProxy;
import org.chromium.brave_wallet.mojom.JsonRpcService;
import org.chromium.brave_wallet.mojom.KeyringService;
import org.chromium.brave_wallet.mojom.SolanaTxManagerProxy;
import org.chromium.brave_wallet.mojom.TxService;

// Under development, some parts not tested so use with caution
// A container for all the native services and APIs
public class WalletModel {
    private KeyringService mKeyringService;
    private BlockchainRegistry mBlockchainRegistry;
    private JsonRpcService mJsonRpcService;
    private TxService mTxService;
    private EthTxManagerProxy mEthTxManagerProxy;
    private SolanaTxManagerProxy mSolanaTxManagerProxy;
    private BraveWalletService mBraveWalletService;
    private AssetRatioService mAssetRatioService;
    private final CryptoModel mCryptoModel;
    private final DappsModel mDappsModel;
    private final KeyringModel mKeyringModel;
    private Context mContext;
    private CryptoActions mCryptoActions;

    public WalletModel(Context context, KeyringService keyringService,
            BlockchainRegistry blockchainRegistry, JsonRpcService jsonRpcService,
            TxService txService, EthTxManagerProxy ethTxManagerProxy,
            SolanaTxManagerProxy solanaTxManagerProxy, AssetRatioService assetRatioService,
            BraveWalletService braveWalletService) {
        mContext = context;
        mKeyringService = keyringService;
        mBlockchainRegistry = blockchainRegistry;
        mJsonRpcService = jsonRpcService;
        mTxService = txService;
        mEthTxManagerProxy = ethTxManagerProxy;
        mSolanaTxManagerProxy = solanaTxManagerProxy;
        mAssetRatioService = assetRatioService;
        mBraveWalletService = braveWalletService;
        mCryptoActions = new CryptoActions();
        mCryptoModel = new CryptoModel(mContext, mTxService, mKeyringService, mBlockchainRegistry,
                mJsonRpcService, mEthTxManagerProxy, mSolanaTxManagerProxy, mBraveWalletService,
                mAssetRatioService, mCryptoActions);
        mDappsModel = new DappsModel(
                mJsonRpcService, mBraveWalletService, mCryptoModel.getPendingTxHelper());
        mKeyringModel = new KeyringModel(
                keyringService, mCryptoModel.getSharedData(), braveWalletService, mCryptoActions);
        // be careful with dependencies, must avoid cycles
        mCryptoModel.setAccountInfosFromKeyRingModel(mKeyringModel.mAccountInfos);
        init();
    }

    public void resetServices(Context context, KeyringService keyringService,
            BlockchainRegistry blockchainRegistry, JsonRpcService jsonRpcService,
            TxService txService, EthTxManagerProxy ethTxManagerProxy,
            SolanaTxManagerProxy solanaTxManagerProxy, AssetRatioService assetRatioService,
            BraveWalletService braveWalletService) {
        mContext = context;
        setKeyringService(keyringService);
        setBlockchainRegistry(blockchainRegistry);
        setJsonRpcService(jsonRpcService);
        setTxService(txService);
        setEthTxManagerProxy(ethTxManagerProxy);
        setSolanaTxManagerProxy(solanaTxManagerProxy);
        setAssetRatioService(assetRatioService);
        setBraveWalletService(braveWalletService);
        mCryptoModel.resetServices(mContext, mTxService, mKeyringService, mBlockchainRegistry,
                mJsonRpcService, mEthTxManagerProxy, mSolanaTxManagerProxy, mBraveWalletService,
                mAssetRatioService);
        mDappsModel.resetServices(
                mJsonRpcService, mBraveWalletService, mCryptoModel.getPendingTxHelper());
        mKeyringModel.resetService(mKeyringService, braveWalletService);
        init();
    }

    /*
     * Explicit method to ensure the safe initialisation to start the required data process
     */
    private void init() {
        mCryptoModel.init();
        mKeyringModel.init();
    }

    public KeyringModel getKeyringModel() {
        return mKeyringModel;
    }

    public boolean hasAllServices() {
        return getKeyringService() != null && getBlockchainRegistry() != null
                && getJsonRpcService() != null && getTxService() != null
                && getEthTxManagerProxy() != null && getSolanaTxManagerProxy() != null
                && getAssetRatioService() != null && getBraveWalletService() != null;
    }

    public CryptoModel getCryptoModel() {
        return mCryptoModel;
    }

    public DappsModel getDappsModel() {
        return mDappsModel;
    }

    public KeyringService getKeyringService() {
        return mKeyringService;
    }

    public void setKeyringService(KeyringService mKeyringService) {
        this.mKeyringService = mKeyringService;
    }

    public BlockchainRegistry getBlockchainRegistry() {
        return mBlockchainRegistry;
    }

    public void setBlockchainRegistry(BlockchainRegistry mBlockchainRegistry) {
        this.mBlockchainRegistry = mBlockchainRegistry;
    }

    public JsonRpcService getJsonRpcService() {
        return mJsonRpcService;
    }

    public void setJsonRpcService(JsonRpcService mJsonRpcService) {
        this.mJsonRpcService = mJsonRpcService;
    }

    public TxService getTxService() {
        return mTxService;
    }

    public void setTxService(TxService mTxService) {
        this.mTxService = mTxService;
    }

    public EthTxManagerProxy getEthTxManagerProxy() {
        return mEthTxManagerProxy;
    }

    public void setEthTxManagerProxy(EthTxManagerProxy mEthTxManagerProxy) {
        this.mEthTxManagerProxy = mEthTxManagerProxy;
    }

    public SolanaTxManagerProxy getSolanaTxManagerProxy() {
        return mSolanaTxManagerProxy;
    }

    public void setSolanaTxManagerProxy(SolanaTxManagerProxy mSolanaTxManagerProxy) {
        this.mSolanaTxManagerProxy = mSolanaTxManagerProxy;
    }

    public BraveWalletService getBraveWalletService() {
        return mBraveWalletService;
    }

    public void setBraveWalletService(BraveWalletService mBraveWalletService) {
        this.mBraveWalletService = mBraveWalletService;
    }

    public AssetRatioService getAssetRatioService() {
        return mAssetRatioService;
    }

    public void setAssetRatioService(AssetRatioService mAssetRatioService) {
        this.mAssetRatioService = mAssetRatioService;
    }

    class CryptoActions implements CryptoSharedActions {
        @Override
        public void updateCoinType() {
            mCryptoModel.updateCoinType();
        }

        /**
         * The network, coin, and account needs to be update whenever any of the other two are
         * changed
         *
         * @param chainId of selected chain
         * @param coin    of current account and network, SOL, ETH etc.
         */
        @Override
        public void updateCoinAccountNetworkInfo(@CoinType.EnumType int coin) {
            // update coin
            mBraveWalletService.setSelectedCoin(coin);
            mCryptoModel.updateCoinType(isCoinUpdated -> {
                // update account per selected coin
                mKeyringModel.update();
                // update network per selected coin
                mCryptoModel.getNetworkModel().init();
            });
        }
    }
}
