/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/logger.h>
#include <memory>
#include <vector>
#include <string>
#include <inja/inja.hpp>
#include <inja/environment.hpp>
#include <inja/template.hpp>

namespace udocs_processor {
class Templater {
 public:
  Templater();

  virtual std::string Render(
      const std::string& Path, const nlohmann::json& Data);

 private:
  nlohmann::json ResourceCallback(
      const std::vector<const nlohmann::json*>& Args) const;

  std::string MakeResource(const std::string& Address) const;

  nlohmann::json UdocsResourceCallback(
      const std::vector<const nlohmann::json*>& Args) const;

  std::string MakeUdocsResource(const std::string& Address) const;

  static const bool DO_LSTRIP_BLOCKS = true;
  static const bool DO_TRIM_BLOCKS = true;
  static constexpr char RESOURCE_CALLBACK[] = "resource";
  static constexpr int RESOURCE_CALLBACK_ARGS_COUNT = 1;
  static constexpr char UDOCS_RESOURCE_CALLBACK[] = "udocs_resource";
  static constexpr int UDOCS_RESOURCE_CALLBACK_ARGS_COUNT = 1;
  static constexpr char DIRECTORY_SEPARATOR = '/';
  static constexpr char LOGGER_NAME[] = "templater";
  static constexpr char RESOURCES_PATH[] = "";

  std::shared_ptr<spdlog::logger> l;

  inja::Environment Env;
};
}  // namespace udocs_processor
