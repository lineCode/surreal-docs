/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <spdlog/sinks/sink.h>
#include <spdlog/logger.h>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <memory>
#include <optional>
#include <nlohmann/json.hpp>
#include "udocs-processor/cli/thirdparty/WinReg.hpp"

namespace udocs_processor {
/// win64 compatible only
class UnrealInteraction {
 public:
  struct UnrealVersion {
    std::string Version;
    std::string EngineDir;
    bool IsCustomBuild = false;
  };

  struct Source {
    std::string Path;
    std::string Module;
    bool IsPlugin = false;
  };

  explicit UnrealInteraction(std::shared_ptr<spdlog::sinks::sink> Sink);

  std::vector<UnrealVersion> EnumerateVersions() const;

  size_t DetermineVersion(const std::string& ProjectDir,
      const std::vector<UnrealVersion>& Versions) const;

  void Build(const std::string& Target, const std::string& Configuration,
      const std::string& ProjectDir, const UnrealVersion& Version) const;

  void Run(const std::string& ProjectDir, const UnrealVersion& Version,
      const std::vector<std::string>& Arguments,
      bool DoUseCmd = false) const;

  bool CheckIfUnrealProjectRoot(const std::string& ProjectDir) const;

  void EnablePlugin(const std::string& ProjectDir,
      const std::string& PluginName) const;

  std::vector<std::string> EnumerateModules(
      const std::string &ProjectDir) const;

  std::string DetermineProjectName(const std::string &ProjectDir) const;

  std::map<std::string, std::optional<std::string>> EnumerateTargets(
      const std::string &ProjectDir) const;

  std::optional<std::string> DetermineLastCompiledTarget(
      const std::string &ProjectDir) const;

  std::vector<Source> EnumerateSources(const std::string &ProjectDir) const;

  void EnableAutoCompile(const std::string &ProjectDir) const;

  bool IsBlueprintProject(const std::string &ProjectDir) const;

  static constexpr const char PLUGINS_DIRECTORY[] = "Plugins";

 private:
  std::set<std::string> FindAllAvailableTargets(
      const std::string& ProjectDir) const;

  uint64_t ParseUATExitCode(const std::string& UATOutput) const;

  std::string FindUeEditor(const std::string& EngineDir, bool DoUseCmd) const;

  nlohmann::json LoadProjectFile(const std::string& ProjectDir) const;

  std::string FindProjectFile(const std::string& ProjectDir) const;

  std::vector<Source> FindModules(const std::string& Dir) const;

  static constexpr const char UAT_EXIT_CODE_PATTERN[] = "ExitCode=(-?\\d+)";
  static constexpr const int UAT_EXIT_CODE_PATTERN_CODE_GROUP = 1;

  static constexpr const int JSON_INDENT = 4;

  static constexpr const char DEFAULT_TARGET[] = "Editor";

  static constexpr const char LOGGER_NAME[] = "unreal-interaction";
  static constexpr char DIRECTORY_SEPARATOR = '/';
  static constexpr char EXTENSION_SEPARATOR = '.';
  static constexpr const char* NEWLINE = "\n";

  static constexpr const char UNREAL_EDITOR_5_CMD_PATH[] =
      "/Engine/Binaries/Win64/UnrealEditor-Cmd.exe";
  static constexpr const char UNREAL_EDITOR_4_CMD_PATH[] =
      "/Engine/Binaries/Win64/UE4Editor-Cmd.exe";
  static constexpr const char UNREAL_EDITOR_5_PATH[] =
      "/Engine/Binaries/Win64/UnrealEditor.exe";
  static constexpr const char UNREAL_EDITOR_4_PATH[] =
      "/Engine/Binaries/Win64/UE4Editor.exe";
  static constexpr const char UNREAL_EDITOR_SKIP_COMPILE[] = "-skipcompile";

  static constexpr const char UNREAL_UAT_PATH[] =
      "/Engine/Build/BatchFiles/RunUAT.bat";
  static constexpr const char UNREAL_UAT_BUILD_COMMAND[] = "BuildTarget";
  static constexpr const char UNREAL_UAT_PROJECT[] = "-project=";
  static constexpr const char UNREAL_UAT_TARGET[] = "-Target=";
  static constexpr const char UNREAL_UAT_PLATFORM_WIN64[] = "-platform=Win64";
  static constexpr const char UNREAL_UAT_CLIENT_CONFIG[] = "-clientconfig=";
  static constexpr const char UNREAL_UAT_BUILD[] = "-build";
  static constexpr const char UNREAL_UAT_NOTOOLS[] = "-notools";

  static constexpr const char WIN64_TARGETS_DIRECTORY[] =
      "Intermediate/Build/Win64";
  static constexpr const char UNREAL_EDITOR_TARGET[] = "UnrealEditor";
  static constexpr const char SOURCE_DIRECTORY[] = "Source";
  static constexpr const char UNREALISTIC_DOCS_PLUGIN[] = "UnrealisticDocs";

  static constexpr const char UNREAL_PROJECT_EXTENSION[] = ".uproject";
  static constexpr const char BUILD_CS_PATTERN[] =
      "([a-z0-9_.$]+)\\.build\\.cs";
  static constexpr const char TARGET_PATTERN[] =
      "([a-z0-9_.$]+)\\.Target\\.cs";
  static constexpr const int BUILD_CS_PATTERN_NAME_INDEX = 1;
  static constexpr const int TARGET_PATTERN_NAME_INDEX = 1;

  static constexpr const char PROJECT_ENGINE_ASSOCIATION[] =
      "EngineAssociation";
  static constexpr const char PROJECT_MODULES[] = "Modules";
  static constexpr const char PROJECT_MODULE_NAME[] = "Name";
  static constexpr const char PROJECT_PLUGINS[] = "Plugins";
  static constexpr const char PROJECT_PLUGIN_NAME[] = "Name";
  static constexpr const char PROJECT_PLUGIN_ENABLED[] = "Enabled";

  static constexpr const char CONFIGURATION_DEBUG[] = "Debug";
  static constexpr const char CONFIGURATION_DEBUG_GAME[] = "DebugGame";
  static constexpr const char CONFIGURATION_DEVELOPMENT[] = "Development";
  static constexpr const char CONFIGURATION_SHIPPING[] = "Shipping";
  static constexpr const char CONFIGURATION_TEST[] = "Test";

  // good enough
  static constexpr const char COMPILE_ENABLED_PATTERN[] =
      "bForceCompilationAtStartup\\s*=\\s*True";

  static constexpr const char PER_PROJECT_EDITOR_SETTINGS_PATH[] =
      "Saved/Config/WindowsEditor/EditorPerProjectUserSettings.ini";
  static constexpr const char AUTO_COMPILE_ON_STARTUP[] =
R"(

[/Script/UnrealEd.EditorLoadingSavingSettings]
bForceCompilationAtStartup=True
)";

  static constexpr const char BACKUP_POSTFIX[] = ".backup";

  static constexpr const wchar_t* UNREAL_BUILDS_REG_KEY =
      LR"(SOFTWARE\Epic Games\Unreal Engine\Builds)";
  static constexpr const wchar_t* UNREAL_VERSIONS_REG_KEY =
      LR"(SOFTWARE\EpicGames\Unreal Engine)";
  static constexpr const wchar_t* UNREAL_VERSION_VALUE_KEY =
      LR"(InstalledDirectory)";
  static constexpr const wchar_t* REGISTRY_KEY_SEPARATOR = LR"(\)";

  std::vector<std::string> Configurations;
  std::shared_ptr<spdlog::logger> l;
};
}  // namespace udocs_processor
