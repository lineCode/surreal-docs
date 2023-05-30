/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/cli/SurDocsCLI.h"
#include "udocs-processor/cli/cli/InitCLI.h"

#undef max
#undef min
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

bool udocs_processor::SurDocsCLI::Start(int Argc, char** Argv) {
  CLI::App App;
  App.description(CLI_DESCRIPTION);
  App.add_flag_callback(GLOBAL_FLAG_NO_TELE, [this](){
    Telemetry->SetTelemetryEnabled(false);
  }, GLOBAL_FLAG_NO_TELE_DESCRIPTION);

  CLI::App* GenerateCommand = App.add_subcommand(
      GENERATE_COMMAND, GENERATE_DESCRIPTION);
  bool IsNonInteractive = false;
  bool DoDeploy = false;
  std::string DirectoryOut;
  GenerateCommand->add_flag<bool>(GENERATE_FLAG_NON_INTERACTIVE,
      IsNonInteractive, GENERATE_FLAG_NON_INTERACTIVE_DESCRIPTION);
  GenerateCommand->add_flag<bool>(GENERATE_FLAG_DO_DEPLOY,
      DoDeploy, GENERATE_FLAG_DO_DEPLOY_DESCRIPTION);
  GenerateCommand->add_option<std::string>(GENERATE_FLAG_OUT_DIRECTORY,
      DirectoryOut, GENERATE_FLAG_OUT_DIRECTORY_DESCRIPTION)->required();
  GenerateCommand->callback(
    [&IsNonInteractive, &DoDeploy, &DirectoryOut, this](){
      GenerateCLI::Arguments Args;
      Args.DoDeploy = DoDeploy;
      Args.IsInteractive = !IsNonInteractive;
      Args.OutDirectory = std::move(DirectoryOut);
      bool Result = Generate->Generate(Args);
      if (!Result) {
        throw std::runtime_error{"Generate returned `false`, propagating"};
      }
    });

  CLI::App* InitCommand = App.add_subcommand(INIT_COMMAND, INIT_DESCRIPTION);
  InitCommand->callback(
    [this]() {
      InitCLI::Arguments Args;
      bool Result = Init->Init(Args);
      if (!Result) {
        throw std::runtime_error{"Init returned `false`, propagating"};
      }
    });

  try {
    App.parse(Argc, Argv);
  } catch (const CLI::ParseError &Exc) {
    l->error("Command Parsing Exception: {}", Exc.what());
    return !App.exit(Exc);
  } catch (const std::exception& Exc) {
    l->error("Exception: {}", Exc.what());
    return false;
  }

  return true;
}

udocs_processor::SurDocsCLI::SurDocsCLI(
    std::shared_ptr<spdlog::sinks::sink> Sink,
    std::unique_ptr<GenerateCLI> Generate,
    std::unique_ptr<InitCLI> Init,
    std::shared_ptr<TelemetryReporter> Telemetry)
    : Generate(std::move(Generate)), Init(std::move(Init)),
    Telemetry(Telemetry) {
  l = spdlog::get(LOGGER_NAME);
  if (!l) {
    l = std::make_shared<spdlog::logger>(LOGGER_NAME);
    if (Sink) {
      l->sinks().emplace_back(Sink);
    }
  }
}
