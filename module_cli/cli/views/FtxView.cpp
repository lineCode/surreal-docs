/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/cli/views/FtxView.h"

#define NOLINT()

std::shared_ptr<ftxui::Node> udocs_processor::FtxView::WrapInWindow(
    const std::vector<std::shared_ptr<ftxui::Node>>& Nodes,
    const std::string &Name) const {
  using namespace ftxui; NOLINT()

  std::vector<std::shared_ptr<ftxui::Node>> ToInsert(Nodes);
  ToInsert.emplace_back(vbox({
      separator(),
      hbox({
          text(COPYRIGHT),
          filler(),
          text("MEDELFOR") | color(Color::White),
          text(".") | color(Color::Red)
      })
  }));

  return window(
      hbox({
          Gap(),
          text(Name) | ForegroundColor1(),
          filler(),
          text(GetNewVersionAnnouncement())
              | ErroneuousColor()
              | HHide(GetNewVersionAnnouncement().empty()),
          filler(),
          text(GetVersion()),
          Gap()
      }),
      vbox(ToInsert)) | BackgroundColor() | ForegroundColor2();
}

std::shared_ptr<ftxui::Node> udocs_processor::FtxView::Gap() const {
  return Gap(STANDARD_GAP_SIZE);
}

std::shared_ptr<ftxui::Node> udocs_processor::FtxView::Gap(size_t Size) const {
  return ftxui::text("") | size(ftxui::WIDTH, ftxui::Constraint::EQUAL, Size);
}

std::shared_ptr<ftxui::Node> udocs_processor::FtxView::WrapError(
    const std::string& ErrorDescription, bool HasErrored) const {
  using namespace ftxui; NOLINT()

  return
     vbox({
         hbox({
             Gap(), text("Failure") | HeadingColor()
         }) | HeadingBackgroundColor(),
         separatorEmpty(),
         hflow(
             Gap(), paragraph(ErrorDescription) | ErroneuousColor()),
         separatorEmpty(),
         hbox({
             Gap(), text("Logs") | HeadingColor()
         }) | HeadingBackgroundColor(),
         hflow(
             paragraph(View::GetLogFilePath()) | ForegroundColor1()),
         hbox({
             Gap(), text("Available Support") | HeadingColor()
         }) | HeadingBackgroundColor(),
         hflow(
             paragraph(ERROR_ISSUE) | ForegroundColor1()),
         separatorEmpty(),
         hflow(
             paragraph(ERROR_SORRY) | ForegroundColor1())
     }) | VHide(!HasErrored);
}

ftxui::Decorator udocs_processor::FtxView::VHide(bool DoHide) const {
  return size(ftxui::HEIGHT, DoHide
      ? ftxui::Constraint::EQUAL
      : ftxui::Constraint::GREATER_THAN, 0);
}

ftxui::Decorator udocs_processor::FtxView::HHide(bool DoHide) const {
  return size(ftxui::WIDTH, DoHide
      ? ftxui::Constraint::EQUAL
      : ftxui::Constraint::GREATER_THAN, 0);
}

ftxui::Decorator udocs_processor::FtxView::BackgroundColor() const {
  return ftxui::bgcolor(BackgroundColor_);
}

ftxui::Decorator udocs_processor::FtxView::ForegroundColor1() const {
  return ftxui::color(ForegroundColor1_);
}

ftxui::Decorator udocs_processor::FtxView::ForegroundColor2() const {
  return ftxui::color(ForegroundColor2_);
}

ftxui::Decorator udocs_processor::FtxView::HeadingColor() const {
  return ftxui::color(HeadingColor_);
}

ftxui::Decorator udocs_processor::FtxView::HeadingBackgroundColor() const {
  return ftxui::bgcolor(HeadingBackgroundColor_);
}

ftxui::Decorator udocs_processor::FtxView::SuccessColor() const {
  return ftxui::color(SuccessColor_);
}

ftxui::Decorator udocs_processor::FtxView::ErroneuousColor() const {
  return ftxui::color(ErroneuousColor_);
}
