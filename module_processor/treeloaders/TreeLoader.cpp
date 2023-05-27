/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/treeloaders/TreeLoader.h"

void udocs_processor::TreeLoader::SetReport(ProcessingReport *Report) {
  this->Report = Report;
}

udocs_processor::ProcessingReport *udocs_processor::TreeLoader::GetReport() {
  return Report;
}

std::string_view udocs_processor::TreeLoader::GetName() const {
  return TREELOADER_NAME;
}
