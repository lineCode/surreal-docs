/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include "udocs-processor/cli/commands/generate/rendering/Templater.h"
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>
#include <inja/template.hpp>

udocs_processor::Templater::Templater() {
  Env.set_lstrip_blocks(DO_LSTRIP_BLOCKS);
  Env.set_trim_blocks(DO_TRIM_BLOCKS);

  Env.add_callback(RESOURCE_CALLBACK, RESOURCE_CALLBACK_ARGS_COUNT,
      [this](const std::vector<const nlohmann::json*>& Args) {
        return ResourceCallback(Args);
  });

  Env.add_callback(UDOCS_RESOURCE_CALLBACK, UDOCS_RESOURCE_CALLBACK_ARGS_COUNT,
      [this](const std::vector<const nlohmann::json*>& Args) {
        return UdocsResourceCallback(Args);
  });

  l = spdlog::get(LOGGER_NAME);
  if (!l) {
    l = spdlog::stdout_color_mt(LOGGER_NAME);
  }
}

std::string udocs_processor::Templater::Render(
    const std::string& Path, const nlohmann::json& Data) {
  return Env.render_file(RESOURCES_PATH + Path, Data);
}

nlohmann::json udocs_processor::Templater::ResourceCallback(
    const std::vector<const nlohmann::json*>& Args) const {
  if (!Args.empty() && Args[0]->is_string()) {
    return MakeResource(*Args[0]);
  }

  return "";
}

std::string udocs_processor::Templater::MakeResource(
    const std::string &Address) const {
  return RESOURCES_PATH + Address;
}

nlohmann::json udocs_processor::Templater::UdocsResourceCallback(
    const std::vector<const nlohmann::json*>& Args) const {
  if (!Args.empty() && Args[0]->is_string()) {
    return MakeUdocsResource(*Args[0]);
  }

  return "";
}

std::string udocs_processor::Templater::MakeUdocsResource(
    const std::string &Address) const {
  return RESOURCES_PATH + Address;
}
