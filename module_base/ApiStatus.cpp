/* Copyright © 2022, Medelför OÜ. All rights reserved. */

#include "udocs-processor/ApiStatus.h"

#include <utility>

udocs_processor::ApiStatus::ApiStatus(int32_t Code, std::string Message)
    : Code(Code), Message(std::move(Message)) {}

int32_t udocs_processor::ApiStatus::GetCode() const {
  return Code;
}

const std::string &udocs_processor::ApiStatus::GetMessageDescription() const {
  return Message;
}
