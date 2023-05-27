/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#define NOLINT()

#include <spdlog/spdlog.h>
#include <fmt/chrono.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <utility>
#include <chrono> NOLINT()
#include <filesystem>
#include "udocs-processor/IdGenerator.h"
#include "udocs-processor/doctree/DocNode.h"
#include "nlohmann/json.hpp"
#include "udocs-processor/UDocsProcessor.h"
#include "udocs-processor/treeloaders/TreeLoader.h"
#include "udocs-processor/report/ReportSpdLogSink.h"
#include "udocs-processor/LoggerRegistrar.h"

udocs_processor::UDocsProcessor::UDocsProcessor(IdGenerator &IdGen,
    std::shared_ptr<spdlog::sinks::sink> Sink)
    : IdGen(IdGen) {
  l = std::make_shared<spdlog::logger>("udocs-processor");
  spdlog::register_logger(l);
  Report = std::make_unique<ProcessingReport>();
  // l->set_level(spdlog::level::err);
  if (Sink) {
    l->sinks().emplace_back(Sink);
  }
  l->sinks().emplace_back(
      std::make_shared<ReportSpdLogSink_mt>(*Report));
}

void udocs_processor::UDocsProcessor::AddTreeLoader(
    std::unique_ptr<TreeLoader> TreeLoader) {
  TreeLoaders.emplace_back(std::move(TreeLoader));
}

void udocs_processor::UDocsProcessor::SetDocTreeSerializer(
    std::unique_ptr<DocTreeSerializer> DocTreeSerializer) {
  SelectedDocTreeSerializer = std::move(DocTreeSerializer);
}

void udocs_processor::UDocsProcessor::SetStatusReporter(
    std::function<void(Status)> ReportCallback) {
  this->ReportCallback = std::move(ReportCallback);
}

void udocs_processor::UDocsProcessor::Process() {
  using time_point = std::chrono::steady_clock::time_point;
  if (TreeLoaders.empty()) {
    l->error("No tree loaders were set. Stopping");

    return;
  }

  if (!SelectedDocTreeSerializer) {
    l->error("No doc tree serializer was set. Stopping");

    return;
  }

  time_point TotalBegin = std::chrono::steady_clock::now();
  l->info("Processing");

  std::shared_ptr<DocNode> Root =
      std::make_shared<DocNode>(std::weak_ptr<DocNode>(), IdGen.ObtainId());

  if (ReportCallback) ReportCallback(Status::LOADING);
  for (auto& TreeLoader : TreeLoaders) {
    time_point Begin = std::chrono::steady_clock::now();
    TreeLoader->SetReport(Report.get());
    TreeLoader->LoadTree(IdGen, Root);
    time_point End = std::chrono::steady_clock::now();

    auto Time =
        std::chrono::duration_cast<std::chrono::milliseconds>(End - Begin);

    l->info("Time taken by `{}` tree loader: {}",
        TreeLoader->GetName(), FormatTime(Time));
  }

  if (ReportCallback) ReportCallback(Status::PROCESSING);
  for (auto& Subprocessor : Subprocessors) {
    time_point Begin = std::chrono::steady_clock::now();
    Subprocessor->SetReport(Report.get());
    Subprocessor->SetRoot(Root);
    bool DoStart = Subprocessor->Before();

    if (DoStart) {
      Root->IterateChildren(*Subprocessor, true);
      Subprocessor->After();
      time_point End = std::chrono::steady_clock::now();

      auto Time =
          std::chrono::duration_cast<std::chrono::milliseconds>(End - Begin);

      l->info("Time taken by `{}` subprocessor: {}",
              Subprocessor->GetName(), FormatTime(Time));
    } else {
      l->warn("`{}` subprocessor refused to start",
          Subprocessor->GetName());
    }
  }

  SelectedDocTreeSerializer->SetLogger(l);
  time_point Begin = std::chrono::steady_clock::now();
  SelectedDocTreeSerializer->SetReport(Report.get());
  SelectedDocTreeSerializer->Serialize(*Root);
  time_point End = std::chrono::steady_clock::now();

  auto Time =
      std::chrono::duration_cast<std::chrono::milliseconds>(End - Begin);

  l->info("Time taken by the serializer: {}", FormatTime(Time));
  time_point TotalEnd = std::chrono::steady_clock::now();
  auto TotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(
      TotalEnd - TotalBegin);
  l->info("UDocs were processed. Time: {}",
      FormatTime(TotalTime));
}

std::string udocs_processor::UDocsProcessor::FormatTime(
    std::chrono::milliseconds Duration) const {
  auto TotalMilliseconds = Duration;

  std::string Result = fmt::format("total time: {:%H:%M:%S}. total ms: {}",
      Duration, Duration.count());
  return Result;
}

const udocs_processor::ProcessingReport &
  udocs_processor::UDocsProcessor::GetReport() const {
  return *Report;
}

void udocs_processor::UDocsProcessor::AddSubprocessor(
    std::unique_ptr<Subprocessor> Subprocessor) {
  Subprocessors.emplace_back(std::move(Subprocessor));
}
