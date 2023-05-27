/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <ftxui/dom/elements.hpp>
#include "View.h"

namespace udocs_processor {
class FtxView : public virtual View {
 public:
  std::shared_ptr<ftxui::Node> WrapInWindow(
      const std::vector<std::shared_ptr<ftxui::Node>>& Nodes,
      const std::string& Name) const;

  std::shared_ptr<ftxui::Node> Gap() const;

  std::shared_ptr<ftxui::Node> Gap(size_t Size) const;

  ftxui::Decorator BackgroundColor() const;

  ftxui::Decorator ForegroundColor1() const;

  ftxui::Decorator ForegroundColor2() const;

  ftxui::Decorator HeadingColor() const;

  ftxui::Decorator HeadingBackgroundColor() const;

  ftxui::Decorator ErroneuousColor() const;

  ftxui::Decorator SuccessColor() const;

  std::shared_ptr<ftxui::Node> WrapError(
      const std::string& ErrorDescription, bool HasErrored) const;

  ftxui::Decorator VHide(bool DoHide) const;

  ftxui::Decorator HHide(bool DoHide) const;

  static constexpr size_t STANDARD_GAP_SIZE = 3;
  const ftxui::Color BackgroundColor_ = ftxui::Color{18, 18, 18};
  // 15, 11, 10};//1, 2, 20};//23, 27, 51};
  const ftxui::Color ForegroundColor1_ = ftxui::Color{201, 201, 201};
  // 230, 181, 133};//153, 158, 207};//145, 162, 250};
  const ftxui::Color ForegroundColor2_ = ftxui::Color{59, 59, 59};

  const ftxui::Color HeadingColor_ = ftxui::Color::White;
  const ftxui::Color HeadingBackgroundColor_ = ftxui::Color{186, 18, 0};
  const ftxui::Color ErroneuousColor_ = ftxui::Color::LightCoral;
  const ftxui::Color SuccessColor_ = ftxui::Color::SeaGreen1;

  // 79, 78, 77};//12, 13, 23};//75, 99, 219};
};
}  // namespace udocs_processor
