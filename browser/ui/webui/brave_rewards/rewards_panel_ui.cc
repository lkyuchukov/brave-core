/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/brave_rewards/rewards_panel_ui.h"

#include <memory>
#include <string>
#include <utility>

#include "brave/browser/brave_rewards/rewards_panel_service.h"
#include "brave/browser/brave_rewards/rewards_panel_service_factory.h"
#include "brave/browser/brave_rewards/rewards_service_factory.h"
#include "brave/browser/brave_rewards/rewards_tab_helper.h"
#include "brave/common/webui_url_constants.h"
#include "brave/components/brave_rewards/resources/grit/brave_rewards_panel_generated_map.h"
#include "brave/components/brave_rewards/resources/grit/brave_rewards_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/webui/favicon_source.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "components/favicon_base/favicon_url_parser.h"
#include "components/grit/brave_components_resources.h"
#include "components/grit/brave_components_strings.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace {

using brave_rewards::RewardsPanelService;
using brave_rewards::RewardsPanelServiceFactory;
using brave_rewards::RewardsServiceFactory;
using brave_rewards::RewardsTabHelper;

/*

Next Steps:

- Create a routing system so that browser code can request the rewards panel to
  be opened. Both the Rewards button(s) and the (last) open rewards panel UI.
  The RewardsService can have a callback for this purpose.
- Allow loading adaptive captcha and requestAdsEnabled from the panel app.
- Trigger saving of publisher icon as necessary (we need to define when that is)
- Rewrite uses of braveRewards.openBrowserActionUI and ShowActionUI.

*/

static constexpr webui::LocalizedString kStrings[] = {
    {"summary", IDS_REWARDS_PANEL_SUMMARY},
    {"tip", IDS_REWARDS_PANEL_TIP},
    {"unverifiedCreator", IDS_REWARDS_PANEL_UNVERIFIED_CREATOR},
    {"verifiedCreator", IDS_REWARDS_PANEL_VERIFIED_CREATOR},
    {"refreshStatus", IDS_REWARDS_PANEL_REFRESH_STATUS},
    {"pendingTipText", IDS_REWARDS_PANEL_PENDING_TIP_TEXT},
    {"pendingTipTitle", IDS_REWARDS_PANEL_PENDING_TIP_TITLE},
    {"pendingTipTitleRegistered",
     IDS_REWARDS_PANEL_PENDING_TIP_TITLE_REGISTERED},
    {"platformPublisherTitle", IDS_REWARDS_PANEL_PLATFORM_PUBLISHER_TITLE},
    {"attention", IDS_REWARDS_PANEL_ATTENTION},
    {"sendTip", IDS_REWARDS_PANEL_SEND_TIP},
    {"includeInAutoContribute", IDS_REWARDS_PANEL_INCLUDE_IN_AUTO_CONTRIBUTE},
    {"monthlyTip", IDS_REWARDS_PANEL_MONTHLY_TIP},
    {"ok", IDS_REWARDS_PANEL_OK},
    {"set", IDS_REWARDS_PANEL_SET},
    {"changeAmount", IDS_REWARDS_PANEL_CHANGE_AMOUNT},
    {"cancel", IDS_REWARDS_PANEL_CANCEL},
    {"grantCaptchaTitle", IDS_REWARDS_GRANT_CAPTCHA_TITLE},
    {"grantCaptchaFailedTitle", IDS_REWARDS_GRANT_CAPTCHA_FAILED_TITLE},
    {"grantCaptchaHint", IDS_REWARDS_GRANT_CAPTCHA_HINT},
    {"grantCaptchaPassedTitleUGP", IDS_REWARDS_GRANT_CAPTCHA_PASSED_TITLE_UGP},
    {"grantCaptchaPassedTextUGP", IDS_REWARDS_GRANT_CAPTCHA_PASSED_TEXT_UGP},
    {"grantCaptchaAmountUGP", IDS_REWARDS_GRANT_CAPTCHA_AMOUNT_UGP},
    {"grantCaptchaPassedTitleAds", IDS_REWARDS_GRANT_CAPTCHA_PASSED_TITLE_ADS},
    {"grantCaptchaPassedTextAds", IDS_REWARDS_GRANT_CAPTCHA_PASSED_TEXT_ADS},
    {"grantCaptchaAmountAds", IDS_REWARDS_GRANT_CAPTCHA_AMOUNT_ADS},
    {"grantCaptchaExpiration", IDS_REWARDS_GRANT_CAPTCHA_EXPIRATION},
    {"grantCaptchaErrorTitle", IDS_REWARDS_GRANT_CAPTCHA_ERROR_TITLE},
    {"grantCaptchaErrorText", IDS_REWARDS_GRANT_CAPTCHA_ERROR_TEXT},
    {"rewardsPaymentCheckStatus", IDS_REWARDS_PAYMENT_CHECK_STATUS},
    {"rewardsPaymentCompleted", IDS_REWARDS_PAYMENT_COMPLETED},
    {"rewardsPaymentPending", IDS_REWARDS_PAYMENT_PENDING},
    {"rewardsPaymentProcessing", IDS_REWARDS_PAYMENT_PROCESSING},
    {"walletAccountLink", IDS_REWARDS_WALLET_ACCOUNT_LINK},
    {"walletAddFunds", IDS_REWARDS_WALLET_ADD_FUNDS},
    {"walletAutoContribute", IDS_REWARDS_WALLET_AUTO_CONTRIBUTE},
    {"walletDisconnected", IDS_REWARDS_WALLET_DISCONNECTED},
    {"walletDisconnectLink", IDS_REWARDS_WALLET_DISCONNECT_LINK},
    {"walletEstimatedEarnings", IDS_REWARDS_WALLET_ESTIMATED_EARNINGS},
    {"walletLogIntoYourAccount", IDS_REWARDS_WALLET_LOG_INTO_YOUR_ACCOUNT},
    {"walletMonthlyTips", IDS_REWARDS_WALLET_MONTHLY_TIPS},
    {"walletOneTimeTips", IDS_REWARDS_WALLET_ONE_TIME_TIPS},
    {"walletPending", IDS_REWARDS_WALLET_PENDING},
    {"walletPendingText", IDS_REWARDS_WALLET_PENDING_TEXT},
    {"walletRewardsFromAds", IDS_REWARDS_WALLET_REWARDS_FROM_ADS},
    {"walletRewardsSummary", IDS_REWARDS_WALLET_REWARDS_SUMMARY},
    {"walletUnverified", IDS_REWARDS_WALLET_UNVERIFIED},
    {"walletVerified", IDS_REWARDS_WALLET_VERIFIED},
    {"walletYourBalance", IDS_REWARDS_WALLET_YOUR_BALANCE},
    {"notificationAddFunds", IDS_REWARDS_NOTIFICATION_ADD_FUNDS},
    {"notificationReconnect", IDS_REWARDS_NOTIFICATION_RECONNECT},
    {"notificationClaimRewards", IDS_REWARDS_NOTIFICATION_CLAIM_REWARDS},
    {"notificationClaimTokens", IDS_REWARDS_NOTIFICATION_CLAIM_TOKENS},
    {"notificationAddFundsTitle", IDS_REWARDS_NOTIFICATION_ADD_FUNDS_TITLE},
    {"notificationAddFundsText", IDS_REWARDS_NOTIFICATION_ADD_FUNDS_TEXT},
    {"notificationAutoContributeCompletedTitle",
     IDS_REWARDS_NOTIFICATION_AUTO_CONTRIBUTE_COMPLETED_TITLE},
    {"notificationAutoContributeCompletedText",
     IDS_REWARDS_NOTIFICATION_AUTO_CONTRIBUTE_COMPLETED_TEXT},
    {"notificationBackupWalletTitle",
     IDS_REWARDS_NOTIFICATION_BACKUP_WALLET_TITLE},
    {"notificationBackupWalletText",
     IDS_REWARDS_NOTIFICATION_BACKUP_WALLET_TEXT},
    {"notificationBackupWalletAction",
     IDS_REWARDS_NOTIFICATION_BACKUP_WALLET_ACTION},
    {"notificationWalletDisconnectedTitle",
     IDS_REWARDS_NOTIFICATION_WALLET_DISCONNECTED_TITLE},
    {"notificationWalletDisconnectedText",
     IDS_REWARDS_NOTIFICATION_WALLET_DISCONNECTED_TEXT},
    {"notificationWalletDisconnectedAction",
     IDS_REWARDS_NOTIFICATION_WALLET_DISCONNECTED_ACTION},
    {"notificationWalletVerifiedTitle",
     IDS_REWARDS_NOTIFICATION_WALLET_VERIFIED_TITLE},
    {"notificationWalletVerifiedText",
     IDS_REWARDS_NOTIFICATION_WALLET_VERIFIED_TEXT},
    {"notificationTokenGrantTitle", IDS_REWARDS_NOTIFICATION_TOKEN_GRANT_TITLE},
    {"notificationAdGrantAmount", IDS_REWARDS_NOTIFICATION_AD_GRANT_AMOUNT},
    {"notificationAdGrantTitle", IDS_REWARDS_NOTIFICATION_AD_GRANT_TITLE},
    {"notificationGrantDaysRemaining",
     IDS_REWARDS_NOTIFICATION_GRANT_DAYS_REMAINING},
    {"notificationInsufficientFundsText",
     IDS_REWARDS_NOTIFICATION_INSUFFICIENT_FUNDS_TEXT},
    {"notificationMonthlyContributionFailedText",
     IDS_REWARDS_NOTIFICATION_MONTHLY_CONTRIBUTION_FAILED_TEXT},
    {"notificationMonthlyContributionFailedTitle",
     IDS_REWARDS_NOTIFICATION_MONTHLY_CONTRIBUTION_FAILED_TITLE},
    {"notificationMonthlyTipCompletedTitle",
     IDS_REWARDS_NOTIFICATION_MONTHLY_TIP_COMPLETED_TITLE},
    {"notificationMonthlyTipCompletedText",
     IDS_REWARDS_NOTIFICATION_MONTHLY_TIP_COMPLETED_TEXT},
    {"notificationPublisherVerifiedTitle",
     IDS_REWARDS_NOTIFICATION_PUBLISHER_VERIFIED_TITLE},
    {"notificationPublisherVerifiedText",
     IDS_REWARDS_NOTIFICATION_PUBLISHER_VERIFIED_TEXT},
    {"notificationPendingTipFailedTitle",
     IDS_REWARDS_NOTIFICATION_PENDING_TIP_FAILED_TITLE},
    {"notificationPendingTipFailedText",
     IDS_REWARDS_NOTIFICATION_PENDING_TIP_FAILED_TEXT},
    {"onboardingEarnHeader", IDS_BRAVE_REWARDS_ONBOARDING_EARN_HEADER},
    {"onboardingEarnText", IDS_BRAVE_REWARDS_ONBOARDING_EARN_TEXT},
    {"onboardingSetupAdsHeader", IDS_BRAVE_REWARDS_ONBOARDING_SETUP_ADS_HEADER},
    {"onboardingSetupAdsSubheader",
     IDS_BRAVE_REWARDS_ONBOARDING_SETUP_ADS_SUBHEADER},
    {"onboardingSetupContributeHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_SETUP_CONTRIBUTE_HEADER},
    {"onboardingSetupContributeSubheader",
     IDS_BRAVE_REWARDS_ONBOARDING_SETUP_CONTRIBUTE_SUBHEADER},
    {"onboardingStartUsingRewards",
     IDS_BRAVE_REWARDS_ONBOARDING_START_USING_REWARDS},
    {"onboardingTakeTour", IDS_BRAVE_REWARDS_ONBOARDING_TAKE_TOUR},
    {"onboardingTerms", IDS_BRAVE_REWARDS_ONBOARDING_TERMS},
    {"onboardingTourBack", IDS_BRAVE_REWARDS_ONBOARDING_TOUR_BACK},
    {"onboardingTourBegin", IDS_BRAVE_REWARDS_ONBOARDING_TOUR_BEGIN},
    {"onboardingTourContinue", IDS_BRAVE_REWARDS_ONBOARDING_TOUR_CONTINUE},
    {"onboardingTourDone", IDS_BRAVE_REWARDS_ONBOARDING_TOUR_DONE},
    {"onboardingTourSkip", IDS_BRAVE_REWARDS_ONBOARDING_TOUR_SKIP},
    {"onboardingTourSkipForNow",
     IDS_BRAVE_REWARDS_ONBOARDING_TOUR_SKIP_FOR_NOW},
    {"onboardingPanelWelcomeHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_WELCOME_HEADER},
    {"onboardingPanelWelcomeText",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_WELCOME_TEXT},
    {"onboardingPanelAdsHeader", IDS_BRAVE_REWARDS_ONBOARDING_PANEL_ADS_HEADER},
    {"onboardingPanelAdsText", IDS_BRAVE_REWARDS_ONBOARDING_PANEL_ADS_TEXT},
    {"onboardingPanelScheduleHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_SCHEDULE_HEADER},
    {"onboardingPanelScheduleText",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_SCHEDULE_TEXT},
    {"onboardingPanelAcHeader", IDS_BRAVE_REWARDS_ONBOARDING_PANEL_AC_HEADER},
    {"onboardingPanelAcText", IDS_BRAVE_REWARDS_ONBOARDING_PANEL_AC_TEXT},
    {"onboardingPanelTippingHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_TIPPING_HEADER},
    {"onboardingPanelTippingText",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_TIPPING_TEXT},
    {"onboardingPanelRedeemHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_REDEEM_HEADER},
    {"onboardingPanelRedeemText",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_REDEEM_TEXT},
    {"onboardingPanelSetupHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_SETUP_HEADER},
    {"onboardingPanelSetupText", IDS_BRAVE_REWARDS_ONBOARDING_PANEL_SETUP_TEXT},
    {"onboardingPanelCompleteHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_COMPLETE_HEADER},
    {"onboardingPanelCompleteText",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_COMPLETE_TEXT},
    {"onboardingPanelVerifyHeader",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_VERIFY_HEADER},
    {"onboardingPanelVerifySubtext",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_VERIFY_SUBTEXT},
    {"onboardingPanelVerifyLater",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_VERIFY_LATER},
    {"onboardingPanelVerifyNow", IDS_BRAVE_REWARDS_ONBOARDING_PANEL_VERIFY_NOW},
    {"onboardingPanelBitflyerNote",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_BITFLYER_NOTE},
    {"onboardingPanelBitflyerText",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_BITFLYER_TEXT},
    {"onboardingPanelBitflyerLearnMore",
     IDS_BRAVE_REWARDS_ONBOARDING_PANEL_BITFLYER_LEARN_MORE},
    {"captchaMaxAttemptsExceededText",
     IDS_REWARDS_CAPTCHA_MAX_ATTEMPTS_EXCEEDED_TEXT},
    {"captchaMaxAttemptsExceededTitle",
     IDS_REWARDS_CAPTCHA_MAX_ATTEMPTS_EXCEEDED_TITLE},
    {"captchaSolvedTitle", IDS_REWARDS_CAPTCHA_SOLVED_TITLE},
    {"captchaSolvedText", IDS_REWARDS_CAPTCHA_SOLVED_TEXT},
    {"captchaContactSupport", IDS_REWARDS_CAPTCHA_CONTACT_SUPPORT},
    {"captchaDismiss", IDS_REWARDS_CAPTCHA_DISMISS},
    {"braveTalkTurnOnRewardsToStartCall",
     IDS_REWARDS_BRAVE_TALK_TURN_ON_REWARDS_TO_START_CALL},
    {"braveTalkBraveRewardsDescription",
     IDS_REWARDS_BRAVE_TALK_BRAVE_REWARDS_DESCRIPTION},
    {"braveTalkTurnOnRewards", IDS_REWARDS_BRAVE_TALK_TURN_ON_REWARDS},
    {"braveTalkOptInRewardsTerms", IDS_REWARDS_BRAVE_TALK_OPT_IN_REWARDS_TERMS},
    {"braveTalkTurnOnPrivateAdsToStartCall",
     IDS_REWARDS_BRAVE_TALK_TURN_ON_PRIVATE_ADS_TO_START_CALL},
    {"braveTalkPrivateAdsDescription",
     IDS_REWARDS_BRAVE_TALK_PRIVATE_ADS_DESCRIPTION},
    {"braveTalkTurnOnPrivateAds", IDS_REWARDS_BRAVE_TALK_TURN_ON_PRIVATE_ADS},
    {"braveTalkCanStartFreeCall", IDS_REWARDS_BRAVE_TALK_CAN_START_FREE_CALL},
    {"braveTalkClickAnywhereToBraveTalk",
     IDS_REWARDS_BRAVE_TALK_CLICK_ANYWHERE_TO_BRAVE_TALK},
    {"braveTalkWantLearnMore", IDS_REWARDS_BRAVE_TALK_WANT_LEARN_MORE}};

std::string TakeRewardsPanelArgs(content::WebUI* web_ui) {
  DCHECK(web_ui);
  auto* panel_service =
      RewardsPanelServiceFactory::GetForProfile(Profile::FromWebUI(web_ui));

  return panel_service ? panel_service->TakePanelArguments() : "";
}

class MessageHandler : public content::WebUIMessageHandler,
                       public RewardsPanelService::Observer {
 public:
  MessageHandler() = default;
  ~MessageHandler() override = default;

  MessageHandler(const MessageHandler&) = delete;
  MessageHandler& operator=(const MessageHandler&) = delete;

  // content::WebUIMessageHandler

  void OnJavascriptAllowed() override {
    auto* panel_service =
        RewardsPanelServiceFactory::GetForProfile(Profile::FromWebUI(web_ui()));

    if (panel_service) {
      panel_service_observation_.Observe(panel_service);
    }
  }

  void OnJavascriptDisallowed() override { panel_service_observation_.Reset(); }

  void RegisterMessages() override {
    RegisterMessage("pageReady", &MessageHandler::HandlePageReady);
    RegisterMessage("showUI", &MessageHandler::HandleShowUI);
    RegisterMessage("hideUI", &MessageHandler::HandleHideUI);
  }

  // brave_rewards::RewardsPanelService::Observer
  bool OnRewardsPanelRequested(Browser* browser) override {
    if (!IsJavascriptAllowed()) {
      NOTREACHED();
      return false;
    }

    auto* panel_service =
        RewardsPanelServiceFactory::GetForProfile(Profile::FromWebUI(web_ui()));

    if (!panel_service) {
      return false;
    }

    std::string args = TakeRewardsPanelArgs(web_ui());
    FireWebUIListener("rewardsPanelRequested", base::Value(std::move(args)));
    return true;
  }

 private:
  template <typename F>
  void RegisterMessage(const std::string& name, F&& fn) {
    web_ui()->RegisterMessageCallback(
        name, base::BindRepeating(fn, base::Unretained(this)));
  }

  void HandlePageReady(const base::Value::List& args) {
    AllowJavascript();
    StartRewards();
  }

  void HandleShowUI(const base::Value::List& args) {
    if (auto* panel_ui = GetRewardsPanelUI()) {
      if (auto embedder = panel_ui->embedder()) {
        embedder->ShowUI();
      }
    }
  }

  void HandleHideUI(const base::Value::List& args) {
    if (auto* panel_ui = GetRewardsPanelUI()) {
      if (auto embedder = panel_ui->embedder()) {
        embedder->CloseUI();
      }
    }
  }

  void StartRewards() {
    auto* rewards_service =
        RewardsServiceFactory::GetForProfile(Profile::FromWebUI(web_ui()));

    if (!rewards_service) {
      return NotifyError("rewards-service-not-available");
    }

    rewards_service->StartProcess(base::BindOnce(
        &MessageHandler::OnRewardsProcessStarted, weak_factory_.GetWeakPtr()));
  }

  void NotifyError(const std::string& type) {
    if (IsJavascriptAllowed()) {
      FireWebUIListener("error", base::Value(type));
    }
  }

  void OnRewardsProcessStarted() {
    if (IsJavascriptAllowed()) {
      FireWebUIListener("rewardsStarted");
    }
  }

  RewardsPanelUI* GetRewardsPanelUI() {
    if (auto* controller = web_ui()->GetController()) {
      return controller->GetAs<RewardsPanelUI>();
    }
    return nullptr;
  }

  RewardsPanelService::Observation panel_service_observation_{this};
  base::WeakPtrFactory<MessageHandler> weak_factory_{this};
};

}  // namespace

RewardsPanelUI::RewardsPanelUI(content::WebUI* web_ui)
    : MojoBubbleWebUIController(web_ui, true) {
  auto* source = content::WebUIDataSource::Create(kBraveRewardsPanelHost);
  source->AddLocalizedStrings(kStrings);
  source->AddString("rewardsPanelArgs", TakeRewardsPanelArgs(web_ui));

  webui::SetupWebUIDataSource(source,
                              base::make_span(kBraveRewardsPanelGenerated,
                                              kBraveRewardsPanelGeneratedSize),
                              IDR_BRAVE_REWARDS_PANEL_HTML);

  content::WebUIDataSource::Add(web_ui->GetWebContents()->GetBrowserContext(),
                                source);

  auto* profile = Profile::FromWebUI(web_ui);
  auto favicon_source = std::make_unique<FaviconSource>(
      profile, chrome::FaviconUrlFormat::kFavicon2);
  content::URLDataSource::Add(profile, std::move(favicon_source));

  web_ui->AddMessageHandler(std::make_unique<MessageHandler>());
}

RewardsPanelUI::~RewardsPanelUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(RewardsPanelUI)
