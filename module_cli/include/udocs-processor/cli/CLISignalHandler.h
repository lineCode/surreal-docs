/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

namespace udocs_processor {
class CLISignalHandler {
 public:
  static void Handle(int Signal);

  static void OverrideHandler();

 private:
  static constexpr const int INTERRUPTED_EXIT_CODE = 2;
};
}  // namespace udocs_processor
