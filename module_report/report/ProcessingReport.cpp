/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#include "udocs-processor/report/ProcessingReport.h"

#include <utility>

void udocs_processor::ProcessingReport::SetOutputImagesCount(uint64_t Count) {
  ImagesCount = Count;
}

void udocs_processor::ProcessingReport::SetOutputPagesCount(uint64_t Count) {
  PagesCount = Count;
}

void udocs_processor::ProcessingReport::SetMembersCount(
    udocs_processor::ReportMemberType Type, uint64_t Count) {
  auto It = MembersCount.find(Type);

  if (It != MembersCount.end()) {
    It->second = Count;
  }

  MembersCount.insert(std::make_pair(Type, Count));
}

void udocs_processor::ProcessingReport::AddUnstructuredLogMessage(
    std::string Message) {
  UnstructuredLog.emplace_back(std::move(Message));
}

void udocs_processor::ProcessingReport::SetProcessingTimeInMilliseconds(
    uint64_t Time) {
  this->Time = Time;
}

void udocs_processor::ProcessingReport::AddLogMessage(
    udocs_processor::StructuredLogType Type, std::string Message) {
  StructuredLog.insert(std::make_pair(Type, std::move(Message)));
}

void udocs_processor::ProcessingReport::AddUndocumentedMember(
    udocs_processor::ReportMemberType Type, std::string Name,
    udocs_processor::MemberLocation Location, std::string Message) {
  UndocumentedMember Member(Type, std::move(Name),
      std::move(Location), std::move(Message));

  UndocumentedMembers.emplace_back(std::move(Member));
}

void udocs_processor::ProcessingReport::IncreaseOutputImagesCount(uint64_t By) {
  ImagesCount += By;
}

void udocs_processor::ProcessingReport::IncreaseOutputPagesCount(uint64_t By) {
  PagesCount += By;
}

void udocs_processor::ProcessingReport::IncreaseMembersCount(
    ReportMemberType Type, uint64_t By) {
  auto It = MembersCount.find(Type);

  if (It != MembersCount.end()) {
    It->second += By;
  }

  MembersCount.insert(std::make_pair(Type, By));
}

udocs_processor::MemberLocation::MemberLocation(
    std::string Path, uint64_t LineNumber) : Path(std::move(Path)),
    LineNumber(LineNumber) {}

udocs_processor::UndocumentedMember::UndocumentedMember(
    udocs_processor::ReportMemberType type, std::string name,
    udocs_processor::MemberLocation location, std::string message)
    : Type(type), Name(std::move(name)), Location(std::move(location)),
    Message(std::move(message)) {}
