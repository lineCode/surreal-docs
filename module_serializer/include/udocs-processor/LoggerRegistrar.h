/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace udocs_processor {
extern void RegisterLogger(std::shared_ptr<spdlog::logger> Logger);
}  // namespace udocs_processor

