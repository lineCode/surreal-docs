/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace udocs_processor {
enum class ReportMemberType {
  Class,
  Interface,
  Enum,
  Struct,
  Function,
  Property,
  Delegate,
  LogCategory,
  Module,
  BpClass,
  BpInterface,
  BpEnum,
  BpStruct,
  BpFunction,
  BpProperty,
};

enum class StructuredLogType {
  Info,
  Warning,
  Error,
  Fatal
};

class MemberLocation {
  // header or content path
 public:
  MemberLocation(std::string Path, uint64_t LineNumber);

 private:
  std::string Path{};
  uint64_t LineNumber = 0;
};

struct UndocumentedMember {
 public:
  UndocumentedMember(ReportMemberType type,
                     std::string name,
                     MemberLocation location,
                     std::string message);
 private:
  ReportMemberType Type = ReportMemberType::Class;
  std::string Name;
  MemberLocation Location;
  std::string Message;
};

class ProcessingReport {
 public:
  void IncreaseOutputImagesCount(uint64_t By);

  void SetOutputImagesCount(uint64_t Count);

  void IncreaseOutputPagesCount(uint64_t By);

  void SetOutputPagesCount(uint64_t Count);

  void IncreaseMembersCount(ReportMemberType Type, uint64_t By);

  void SetMembersCount(ReportMemberType Type, uint64_t Count);

  void AddUnstructuredLogMessage(std::string Message);

  void SetProcessingTimeInMilliseconds(uint64_t Time);

  void AddLogMessage(StructuredLogType Type, std::string Message);

  void AddUndocumentedMember(ReportMemberType Type, std::string Name,
                             MemberLocation Location, std::string Message);

 private:
  uint64_t ImagesCount = 0;
  uint64_t PagesCount = 0;
  std::unordered_map<ReportMemberType, uint64_t> MembersCount;
  std::vector<std::string> UnstructuredLog;
  uint64_t Time = 0;
  std::unordered_map<StructuredLogType, std::string> StructuredLog;
  std::vector<UndocumentedMember> UndocumentedMembers;
};
}  // namespace udocs_processor
