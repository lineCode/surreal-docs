/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <spdlog/spdlog.h>
#include "udocs-processor/cli/commands/GenerateCommand.h"
#include "udocs-processor/cli/commands/generate/BpPreparer.h"
#include "udocs-processor/StringHelper.h"

udocs_processor::GenerateCommand::GenerateCommand(
    std::shared_ptr<spdlog::sinks::sink> Sink, std::unique_ptr<BpPreparer> Bp,
    std::unique_ptr<CppPreparer> Cpp, std::unique_ptr<Generator> Gen)
    : Bp(std::move(Bp)), Cpp(std::move(Cpp)), Gen(std::move(Gen)) {
  l = spdlog::get(LOGGER_NAME);
  if (!l) {
    l = std::make_shared<spdlog::logger>(LOGGER_NAME);
    if (Sink) {
      l->sinks().emplace_back(Sink);
    }
  }

  Formats.insert(std::make_pair(GenerateRequest::ExportFormat::HTML,
      Generator::GenerationRequest::ExportFormat::HTML));
  Formats.insert(std::make_pair(GenerateRequest::ExportFormat::DEPLOYMENT,
      Generator::GenerationRequest::ExportFormat::DEPLOYMENT));

  BpStatusToStatus.insert(std::make_pair(BpPreparer::Status::EXTRACTING_INFO,
      Status::EXTRACTING_FROM_BP));
  BpStatusToStatus.insert(std::make_pair(BpPreparer::Status::COMPILING,
      Status::COMPILING));

  GenStatusToStatus.insert(std::make_pair(Generator::Status::LOADING,
      Status::LOADING));
  GenStatusToStatus.insert(std::make_pair(Generator::Status::PROCESSING,
      Status::PROCESSING));
  GenStatusToStatus.insert(std::make_pair(
      Generator::Status::SERIALIZING_INTERNAL, Status::SERIALIZING_INTERNAL));
  GenStatusToStatus.insert(std::make_pair(Generator::Status::SERIALIZING_IMAGES,
      Status::SERIALIZING_IMAGES));
  GenStatusToStatus.insert(std::make_pair(Generator::Status::SERIALIZING_HTML,
      Status::SERIALIZING_HTML));
  GenStatusToStatus.insert(std::make_pair(Generator::Status::FINALIZING,
      Status::FINALIZING));
  GenStatusToStatus.insert(std::make_pair(Generator::Status::CLEANING_UP,
      Status::CLEANING_UP));
  GenStatusToStatus.insert(std::make_pair(Generator::Status::FINISHED,
      Status::CLEANING_UP));  // not a mistake
  GenStatusToStatus.insert(std::make_pair(Generator::Status::ERROR,
      Status::ERROR));
}

/*
 * 1. run bp prepare
 * 2. run cpp prepare
 * 3. run generator
 */
void udocs_processor::GenerateCommand::Generate(
    const GenerateRequest &Request, const CallbackType& StatusCallback) const {
  BpPreparer::PrepareRequest BpRequest;
  BpRequest.Version = {
      "", ConvertToAbsolutePath(Request.EngineRoot)
  };
  BpRequest.ProjectDirectory =
      ConvertToAbsolutePath(Request.ProjectDirectory);
  BpRequest.OutDirectory = ConvertToAbsolutePath(Request.OutDirectory);
  BpRequest.ContentInclude = Request.ContentInclude;
  BpRequest.ContentExclude = Request.ContentExclude;
  BpRequest.NativeInclude = Request.NativeInclude;
  BpRequest.NativeExclude = Request.NativeExclude;
  BpRequest.Configuration = Request.Configuration;
  BpRequest.Target = Request.Target;
  BpRequest.DoUseCmd = Request.DoUseCmd;
  Bp->Prepare(BpRequest, [this, &StatusCallback](BpPreparer::Status Status) {
    auto It = BpStatusToStatus.find(Status);
    if (It != BpStatusToStatus.end()) {
      StatusCallback(It->second, "");
    }
  });

  StatusCallback(Status::EXTRACTING_FROM_CPP, "");
  CppPreparer::PrepareRequest CppRequest;
  CppRequest.OutDirectory = BpRequest.OutDirectory;
  CppRequest.Inputs = Request.CppInputs;
  CppRequest.Exclude = Request.CppExclude;
  Cpp->Prepare(CppRequest);

  Generator::GenerationRequest GenRequest;
  GenRequest.DoExportPrivate = Request.DoExportPrivate;
  GenRequest.OutDirectory = BpRequest.OutDirectory;
  GenRequest.InputDocs = ConvertToAbsolutePath(Request.SurDocsDirectory);
  GenRequest.InputUData = GenRequest.OutDirectory + UDATA_AT;
  GenRequest.InputDoxy = GenRequest.OutDirectory + DOXY_JSON_AT;
  auto It = Formats.find(Request.Format);
  if (It != Formats.end()) {
    GenRequest.Format = It->second;
  } else {
    l->error("Invalid export format {}", static_cast<uint32_t>(Request.Format));
    throw std::invalid_argument{"Invalid format"};
  }
  Gen->Generate(GenRequest,
    [this, &StatusCallback]
    (Generator::Status Status, const std::string& Message) {
      auto It = GenStatusToStatus.find(Status);
      if (It != GenStatusToStatus.end()) {
        StatusCallback(It->second, Message);
      }
    });
}

void udocs_processor::GenerateCommand::CleanUp(
    const std::string& OutDirectory) const {
  std::string AbsPath = ConvertToAbsolutePath(OutDirectory);
  Bp->CleanUp(OutDirectory);
  Cpp->CleanUp(OutDirectory);
  Gen->CleanUp(OutDirectory);
}

std::string udocs_processor::GenerateCommand::ConvertToAbsolutePath(
    const std::string& Path) const {
  return StringHelper::Normalize(std::filesystem::absolute(
      std::filesystem::path{Path}).string());
}
