/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include "udocs-processor/version.h"

namespace udocs_processor {
class View {
 public:
  View();

  virtual void Init() = 0;

  virtual void Tick() = 0;

  virtual void Destroy() = 0;

  void SetLogFilePath(std::string LogFilePath);

  void SetNewVersion(std::string NewVersion);

  std::string GetVersion() const;

  std::string GetLogFilePath() const;

  std::string GetNewVersionAnnouncement() const;

  static constexpr const char COPYRIGHT[] =
      "Copyright (c) 2022-2023, Medelfor, Limited. All rights reserved.";
  static constexpr const char ERROR_ISSUE[] =
      "If you believe something's wrong with the product, please create an "
      "issue in Surreal Docs GitHub available at "
      "https://github.com/medelfor/surreal-docs";
  static constexpr const char ERROR_SORRY[] =
      "\"Surreal Docs Crypto\", \"Surreal Docs XL\" or \"Surreal Docs Crypto "
      "XL\" licenses include Priority Customer Care plan. If you or your "
      "organization own one of these licenses please contact us at "
      "support@medelfor.com with your license ID and problem description, "
      "we will help you to resolve the issue as soon as possible";

 private:
  std::string NewVersion;
  std::string LogFilePath;
  std::string Version;
  static constexpr const char STORED_LOG_PATTERN[] =
      "Current session log is stored at {}. You can find more details about "
      "the issue there";
  static constexpr const char VERSION_PATTERN[] = "Surreal Docs v{}.{}.{}";
  static constexpr const char VERSION_TAG_PATTERN[] = "-{}";

  static constexpr const char NEW_VERSION_PATTERN[] =
      "Surreal Docs v{} is out!";
};
}  // namespace udocs_processor
