/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/cli/CLISignalHandler.h"
#include <spdlog/spdlog.h>
#include <csignal>
#include <cstdlib>

// bad but good enough for now
void udocs_processor::CLISignalHandler::Handle(int Signal) {
  spdlog::shutdown();
  std::exit(INTERRUPTED_EXIT_CODE);
}

void udocs_processor::CLISignalHandler::OverrideHandler() {
  std::signal(SIGTERM, Handle);
  std::signal(SIGINT, Handle);
}
