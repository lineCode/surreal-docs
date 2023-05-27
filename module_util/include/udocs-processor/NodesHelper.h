/* Copyright © 2022, Medelfor, Limited. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include "udocs-processor/doctree/DocNode.h"

namespace udocs_processor {
class BlueprintDelegateTypeNode;
class NativePropertyNode;

class NodesHelper {
 public:
  static bool IsGlobalVariable(const NativePropertyNode& Node);

  static std::shared_ptr<DocNode> FindModule(DocNode& Node);

  static void ExtractLegacyUeEnum(const std::string& Namespace,
      std::string& TrueNamespace, std::string& NameNamespace);

  static std::string FormatNumber(double Value);

  static bool ParseParameterXYZInitializer(const std::string& Initializer,
      double& X, double& Y, double& Z);

  static bool ParseRotatorInitializer(const std::string& Rotator,
      double& X, double& Y, double& Z);

  static bool ParseVectorInitializer(const std::string& Vector,
      double& X, double& Y, double& Z);

  static std::string MakeDisplayName(const BlueprintDelegateTypeNode& Type);

  constexpr static char VECTOR_PATH[] = "/Script/CoreUObject.Vector";
  constexpr static char ROTATOR_PATH[] = "/Script/CoreUObject.Rotator";
  constexpr static char TRANSFORM_PATH[] = "/Script/CoreUObject.Transform";

 private:
  constexpr static char NAMESPACE_SEPARATOR[] = "::";

  constexpr static char ROTATOR_FORMAT[] =
      R"(\(Pitch\s*=\s*([-0-9.]+),Yaw\s*=\s*([-0-9.]+),)"
      R"(Roll\s*=\s*([-0-9.]+)\))";
  constexpr static char VECTOR_FORMAT[] =
      R"(\(X\s*=\s*([-0-9.]+),Y\s*=\s*([-0-9.]+),)"
      R"(Z\s*=\s*([-0-9.]+)\))";
  constexpr static char XYZ_PARAMETER_FORMAT[] =
      R"(\s*([-0-9.]+),([-0-9.]+),([-0-9.]+))";

  constexpr static char FLOATING_POINT[] = ".";
  constexpr static char INTEGER_POSTFIX[] = ".0";
};
}  // namespace udocs_processor
