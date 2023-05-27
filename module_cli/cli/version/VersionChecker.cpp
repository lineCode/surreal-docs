/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/cli/version/VersionChecker.h"
#include <spdlog/spdlog.h>
#include <utility>
#include <fstream>
#include <nlohmann/json.hpp>
#include "udocs-processor/StringHelper.h"

udocs_processor::VersionChecker::VersionChecker(
    std::shared_ptr<spdlog::sinks::sink> Sink, std::string ProductId,
    uint64_t CurrentMajor, uint64_t CurrentMinor, uint64_t CurrentIndex,
    uint64_t CurrentBuildNumber)
    : ProductId(std::move(ProductId)), CurrentMajor(CurrentMajor),
      CurrentMinor(CurrentMinor), CurrentIndex(CurrentIndex),
      CurrentBuildNumber(CurrentBuildNumber) {
  l = spdlog::get(LOGGER_NAME);
  if (!l) {
    l = std::make_shared<spdlog::logger>(LOGGER_NAME);
    if (Sink) {
      l->sinks().emplace_back(Sink);
    }
  }
}

void udocs_processor::VersionChecker::UpdateVersion() {
  uint64_t CurrentTime = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();

  bool DoRecheckVersion = false;
  if (LoadSelf()) {
    if (LastCheckedOn + VERSION_POLL_INTERVAL_S < CurrentTime) {
      DoRecheckVersion = true;
      l->info("Version cache is outdated, rechecking");
    }
  } else {
    DoRecheckVersion = true;
    l->info("No version cache found, rechecking");
  }

  if (DoRecheckVersion) {
    try {
      ProductService::LatestVersionResponse Response =
          Product->GetLatestVersion(ProductId);
      if (Response.Status.GetCode() != ApiStatus::SUCCESS) {
        l->error("Error when rechecking version: {}/{}",
            Response.Status.GetCode(), Response.Status.GetMessageDescription());
        return;
      }

      LatestVersion = Response.Version;
      LastCheckedOn = CurrentTime;

      if (Response.Major > CurrentMajor ||
          (Response.Major == CurrentMajor && Response.Minor > CurrentMinor) ||
          (Response.Major == CurrentMajor && Response.Minor == CurrentMinor &&
           Response.Index > CurrentIndex) ||
          (Response.Major == CurrentMajor && Response.Minor == CurrentMinor &&
           Response.Index == CurrentIndex &&
           Response.BuildNumber > CurrentBuildNumber)) {
        IsOutdated_ = true;
        l->info("Current version is outdated. The new one is {}",
            LatestVersion);
      } else {
        IsOutdated_ = false;
        l->info("You're up-to-date. Latest version: {}", Response.Version);
      }

      SaveSelf();
    } catch (const std::exception& Exc) {
      l->error("Exception when rechecking version: ", Exc.what());
    }
  } else {
    l->error("Version cache is not outdated, skipping recheck");
  }
}

std::optional<std::string> udocs_processor::VersionChecker::IsOutdated() const {
  if (IsOutdated_) {
    return LatestVersion;
  }

  return {};
}

bool udocs_processor::VersionChecker::LoadSelf() {
  std::string Path = CachePath + VERSION_CACHE_FILENAME;
  std::ifstream Input{Path};
  if (!Input.good()) {
    l->error("Couldn't load version check info from `{}`", Path);
    return false;
  }

  nlohmann::json Data = nlohmann::json::parse(Input);

  IsOutdated_ = Data[VERSION_CACHE_IS_OUTDATED];
  LastCheckedOn = Data[VERSION_CACHE_CHECKED_ON];
  LatestVersion = Data[VERSION_CACHE_NEW_VERSION];

  return true;
}

void udocs_processor::VersionChecker::SaveSelf() const {
  std::string Path = CachePath + VERSION_CACHE_FILENAME;
  std::ofstream Output{Path};
  if (!Output.good()) {
    l->error("Couldn't save version check info to `{}`", Path);
    return;
  }

  nlohmann::json Data;
  Data[VERSION_CACHE_CHECKED_ON] = LastCheckedOn;
  Data[VERSION_CACHE_IS_OUTDATED] = IsOutdated_;
  Data[VERSION_CACHE_NEW_VERSION] = LatestVersion;

  Output << Data.dump();
}

void udocs_processor::VersionChecker::SetProductService(
    std::shared_ptr<ProductService> Product) {
  this->Product = Product;
}

void udocs_processor::VersionChecker::SetCachePath(
    const std::string& CachePath) {
  this->CachePath = StringHelper::Normalize(CachePath);
}
