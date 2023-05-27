/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include <fmt/format.h>
#include <ostream>
#include "udocs-processor/cli/views/NonInteractiveGenerateView.h"

void udocs_processor::NonInteractiveGenerateView::SetStatus(
    GenerateView::Status Status_, const std::string &Message) {
  std::scoped_lock<std::mutex> Lock{ToRenderProtection};

  if (HasErrored) return;

  std::string StatusString = GetStatusString(Status_);
  if (Status_ != GenerateView::Status::ERROR) {
    Progress = static_cast<uint32_t>(Status_) / static_cast<float>(
        static_cast<uint32_t>(Status::ERROR) - 1);
  } else {
    HasErrored = true;
  }

  std::string Rendered = fmt::format("[{:^5}] {}\n",
      fmt::format("{:.0f}%", Progress * 100), StatusString);
  ToRender.emplace_back(std::move(Rendered));

  if (Status_ == GenerateView::Status::ERROR) {
    ToRender.emplace_back(fmt::format("{}\n\n", ERROR_ISSUE));
    ToRender.emplace_back(fmt::format("{}\n\n", GetLogFilePath()));
    ToRender.emplace_back(fmt::format("{}\n", ERROR_SORRY));
  }
}

void udocs_processor::NonInteractiveGenerateView::SetFinished(
    bool HasFinished) {
  this->HasFinished = HasFinished;
}

bool udocs_processor::NonInteractiveGenerateView::DoExit() const {
  return HasFinished;
}

udocs_processor::NonInteractiveGenerateView::NonInteractiveGenerateView(
    std::ostream &OutputStream) : OutputStream(OutputStream) {}

void udocs_processor::NonInteractiveGenerateView::Init() {
  OutputStream << fmt::format("{} {}\n", COPYRIGHT, GetVersion());
  if (auto NewVersion = GetNewVersionAnnouncement(); !NewVersion.empty()) {
    OutputStream << std::endl << NewVersion << std::endl << std::endl;
  }
}

void udocs_processor::NonInteractiveGenerateView::Tick() {
  std::lock_guard<std::mutex> Lock{ToRenderProtection};

  for (const auto& Line : ToRender) {
    OutputStream << Line;
  }

  OutputStream.flush();
  ToRender.clear();
}

void udocs_processor::NonInteractiveGenerateView::Destroy() {}
