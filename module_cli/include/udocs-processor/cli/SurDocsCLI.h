/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include "udocs-processor/cli/cli/GenerateCLI.h"
#include "udocs-processor/cli/cli/InitCLI.h"

namespace udocs_processor {
class SurDocsCLI {
 public:
  SurDocsCLI(std::shared_ptr<spdlog::sinks::sink> Sink,
      std::unique_ptr<GenerateCLI> Generate,
      std::unique_ptr<InitCLI> Init,
      std::shared_ptr<TelemetryReporter> Telemetry);

  bool Start(int Argc, char** Argv);

 private:
  static constexpr const char* LOGGER_NAME = "cli";
  static constexpr const char* CLI_DESCRIPTION = "Command line interface to "
      "interact with Surreal Docs. Most of the commands expect working "
      "directory to be a root of an Unreal Engine project";

  static constexpr const char* GLOBAL_FLAG_NO_TELE = "--no-tele";
  static constexpr const char* GLOBAL_FLAG_NO_TELE_DESCRIPTION =
      "Disables sending anonymized usage data to Medelfor during execution "
      "of the current command";

  static constexpr const char* GENERATE_FLAG_OUT_DIRECTORY = "out";
  static constexpr const char* GENERATE_FLAG_OUT_DIRECTORY_DESCRIPTION =
      "Directory where to dump results of docs generation";
  static constexpr const char* GENERATE_FLAG_DO_DEPLOY = "-d,--deploy";
  static constexpr const char* GENERATE_FLAG_DO_DEPLOY_DESCRIPTION =
      "After generation is done, start uploading documentation to the "
      "Surreal Cloud";
  static constexpr const char* GENERATE_FLAG_NON_INTERACTIVE = "-n,--non-rich";
  static constexpr const char* GENERATE_FLAG_NON_INTERACTIVE_DESCRIPTION =
      "Fallback to non-rich UI. Recommended to add when calling from a script";
  static constexpr const char* GENERATE_COMMAND = "generate";
  static constexpr const char* GENERATE_DESCRIPTION =
      "Start docs generation";

  static constexpr const char* INIT_COMMAND = "init";
  static constexpr const char* INIT_DESCRIPTION =
      "Initialize/reconfigure Surreal Docs for this Unreal Engine project";

  std::shared_ptr<spdlog::logger> l;
  std::unique_ptr<GenerateCLI> Generate;
  std::unique_ptr<InitCLI> Init;
  std::shared_ptr<TelemetryReporter> Telemetry;
};
}  // namespace udocs_processor
