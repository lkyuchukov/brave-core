/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/brave_federated/federated_internals_page_handler.h"

#include <string>
#include <utility>
#include <vector>

#include "base/bind.h"
#include "brave/browser/brave_federated/brave_federated_service_factory.h"
#include "brave/components/brave_federated/brave_federated_service.h"
#include "brave/components/brave_federated/data_store_service.h"
#include "brave/components/brave_federated/data_stores/data_store.h"
#include "brave/components/brave_federated/public/interfaces/brave_federated.mojom.h"
#include "chrome/browser/profiles/profile.h"

#include <iostream>

FederatedInternalsPageHandler::FederatedInternalsPageHandler(
    mojo::PendingReceiver<federated_internals::mojom::PageHandler> receiver,
    mojo::PendingRemote<federated_internals::mojom::Page> page,
    Profile* profile)
    : receiver_(this, std::move(receiver)),
      page_(std::move(page)),
      data_store_service_(
          brave_federated::BraveFederatedServiceFactory::GetForBrowserContext(
              profile)
              ->GetDataStoreService()) {}

FederatedInternalsPageHandler::~FederatedInternalsPageHandler() {}

void FederatedInternalsPageHandler::GetDataStoreInfo() {
  if (!data_store_service_)
    return;
  auto* const ad_notification_data_store =
      data_store_service_->GetDataStore(kAdNotificationTaskName);

  std::cerr << "**: " << ad_notification_data_store << std::endl;

  ad_notification_data_store->LoadTrainingData(
      base::BindOnce(&FederatedInternalsPageHandler::OnDataStoreInfoAvailable,
                     weak_ptr_factory_.GetWeakPtr()));
}

void FederatedInternalsPageHandler::OnDataStoreInfoAvailable(
    brave_federated::DataStore::TrainingData training_data) {
  std::cerr << "**: Callback called." << std::endl;
  std::vector<federated_internals::mojom::TrainingInstancePtr> training_instances;
  for (auto const& object : training_data) {
    auto training_instance = federated_internals::mojom::TrainingInstance::New();
    for (auto const& cov : object.second) {
      auto covariate = federated_internals::mojom::Covariate::New();
      covariate->value = cov.value;
      covariate->feature_name = "feature";
      training_instance->covariates.push_back(std::move(covariate));
    }

    training_instances.emplace_back(std::move(training_instance));
  }
  page_->OnDataStoreInfoAvailable(std::move(training_instances));
}
