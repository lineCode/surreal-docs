/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/serializers/DocTreeSerializer.h"

void udocs_processor::DocTreeSerializer::SetReport(
    ProcessingReport *Report) {
  this->Report = Report;
}

udocs_processor::ProcessingReport *
  udocs_processor::DocTreeSerializer::GetReport() {
  return Report;
}
