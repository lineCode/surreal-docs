/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/LoggerRegistrar.h"

void udocs_processor::RegisterLogger(std::shared_ptr<spdlog::logger> Logger) {
  spdlog::register_logger(Logger);
}
