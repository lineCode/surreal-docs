#
#  Copyright © 2022, Medelfor, Limited. All rights reserved.
#  For any additional information refer to https://medelfor.com
#

project(udocs-processor_module_base)

add_library(${PROJECT_NAME} SHARED
        include/udocs-processor/doctree/DocNode.h
        doctree/DocNode.cpp
        include/udocs-processor/doctree/DocNodeVisitor.h
        IdGenerator.cpp
        include/udocs-processor/IdGenerator.h
        include/udocs-processor/doctree/blueprints/BlueprintModuleNode.h
        include/udocs-processor/doctree/blueprints/BlueprintClassNode.h
        include/udocs-processor/doctree/blueprints/BlueprintFunctionNode.h
        include/udocs-processor/doctree/blueprints/BlueprintPropertyNode.h
        include/udocs-processor/doctree/blueprints/BlueprintStructNode.h
        include/udocs-processor/doctree/blueprints/BlueprintEnumNode.h
        include/udocs-processor/doctree/native/NativeModuleNode.h
        include/udocs-processor/doctree/native/NativeClassNode.h
        include/udocs-processor/doctree/native/NativeFunctionNode.h
        include/udocs-processor/doctree/native/DelegateTypeNode.h
        include/udocs-processor/doctree/native/LogCategoryNode.h
        include/udocs-processor/doctree/native/NativePropertyNode.h
        include/udocs-processor/doctree/native/NativeEnumNode.h
        include/udocs-processor/doctree/native/NativeParameterNode.h
        include/udocs-processor/doctree/blueprints/BlueprintParameterNode.h
        doctree/blueprints/BlueprintUNode.cpp
        include/udocs-processor/doctree/blueprints/BlueprintUNode.h
        doctree/native/DelegateTypeNode.cpp
        doctree/native/LogCategoryNode.cpp
        doctree/native/NativeClassNode.cpp
        doctree/native/NativeEnumNode.cpp
        doctree/native/NativeFunctionNode.cpp
        doctree/native/NativeModuleNode.cpp
        doctree/native/NativeParameterNode.cpp
        doctree/native/NativePropertyNode.cpp
        doctree/blueprints/BlueprintClassNode.cpp
        doctree/blueprints/BlueprintEnumNode.cpp
        doctree/blueprints/BlueprintFunctionNode.cpp
        doctree/blueprints/BlueprintModuleNode.cpp
        doctree/blueprints/BlueprintParameterNode.cpp
        doctree/blueprints/BlueprintPropertyNode.cpp
        doctree/blueprints/BlueprintStructNode.cpp
        doctree/native/NativeEnumElementNode.cpp
        include/udocs-processor/doctree/native/NativeEnumElementNode.h
        doctree/blueprints/BlueprintEnumElementNode.cpp
        include/udocs-processor/doctree/blueprints/BlueprintEnumElementNode.h
        include/udocs-processor/doctree/native/ClassParentNode.h
        include/udocs-processor/doctree/AccessModifier.h
        doctree/native/UeInfoNode.cpp
        include/udocs-processor/doctree/native/UeInfoNode.h
        include/udocs-processor/doctree/SeeNode.h
        include/udocs-processor/doctree/native/NativeTypeNode.h
        include/udocs-processor/doctree/native/NativeInitializerNode.h
        doctree/native/NativeInitializerNode.cpp
        include/udocs-processor/doctree/blueprints/BlueprintInitializerNode.h
        doctree/blueprints/BlueprintInitializerNode.cpp
        include/udocs-processor/doctree/native/TemplateNode.h
        doctree/ReferencableWordNode.cpp
        include/udocs-processor/doctree/ReferencableWordNode.h
        include/udocs-processor/doctree/blueprints/types/BlueprintArrayTypeNode.h
        doctree/blueprints/types/BlueprintArrayTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintDelegateTypeNode.h
        doctree/blueprints/types/BlueprintDelegateTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintEnumTypeNode.h
        doctree/blueprints/types/BlueprintEnumTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintInterfaceTypeNode.h
        doctree/blueprints/types/BlueprintInterfaceTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintMapTypeNode.h
        doctree/blueprints/types/BlueprintMapTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintObjectTypeNode.h
        doctree/blueprints/types/BlueprintObjectTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintPrimitiveTypeNode.h
        doctree/blueprints/types/BlueprintPrimitiveTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintSetTypeNode.h
        doctree/blueprints/types/BlueprintSetTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintStructTypeNode.h
        doctree/blueprints/types/BlueprintStructTypeNode.cpp
        include/udocs-processor/doctree/blueprints/types/BlueprintTypeNode.h
        include/udocs-processor/doctree/blueprints/BlueprintClassParentNode.h
        doctree/native/NativeMemberNode.cpp
        include/udocs-processor/doctree/native/NativeMemberNode.h
        doctree/blueprints/types/BlueprintTypeNode.cpp
        doctree/SeeNode.cpp
        doctree/native/NativeTypeNode.cpp
        doctree/native/TemplateNode.cpp
        include/udocs-processor/doctree/NodeTypeChecker.h
        include/udocs-processor/doctree/HasCounterpart.h
        include/udocs-processor/doctree/blueprints/NativeClassCounterpart.h
        doctree/blueprints/NativeClassCounterpart.cpp
        include/udocs-processor/doctree/native/BlueprintableNode.h
        doctree/native/BlueprintableNode.cpp
        include/udocs-processor/doctree/docs/DocsIndexNode.h
        include/udocs-processor/doctree/docs/DocsPageNode.h
        include/udocs-processor/doctree/docs/DocsImageNode.h
        doctree/docs/DocsImageNode.cpp
        doctree/docs/DocsIndexNode.cpp
        doctree/docs/DocsPageNode.cpp
        doctree/docs/DocsGroupNode.cpp
        include/udocs-processor/doctree/docs/DocsGroupNode.h
        include/udocs-processor/doctree/docs/DocsElementNode.h
        doctree/docs/DocsElementNode.cpp
        include/udocs-processor/DocPathsCache.h
        DocPathsCache.cpp
        ApiStatus.cpp
        include/udocs-processor/ApiStatus.h
        )

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "M-UDocs-Base")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,$ORIGIN/")

set(UNIX_LIBRARIES "")

if(PLATFORM_UNIX)
    set(UNIX_LIBRARIES pthread)
endif()

target_link_libraries(${PROJECT_NAME}
        ${UNIX_LIBRARIES}
        spdlog::spdlog
        stringtoolbox::stringtoolbox)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE "")

# each DLL should install it to dist and to dist_conan

# dll, so
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX})

if(DO_INSTALL_AS_CONAN_PACKAGE)
    set(INSTALL_CONAN_PACKAGE_TO ${CMAKE_INSTALL_PREFIX}${CONAN_PACKAGE_INSTALL_POSTFIX})

    install(TARGETS ${PROJECT_NAME}
            ARCHIVE DESTINATION ${INSTALL_CONAN_PACKAGE_TO}/lib
            LIBRARY DESTINATION ${INSTALL_CONAN_PACKAGE_TO}/bin
            RUNTIME DESTINATION ${INSTALL_CONAN_PACKAGE_TO}/bin
            )
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
            DESTINATION ${INSTALL_CONAN_PACKAGE_TO}/include/)
endif()
