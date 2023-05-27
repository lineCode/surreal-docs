#
#  Copyright © 2022, Medelfor, Limited. All rights reserved.
#  For any additional information refer to https://medelfor.com
#

project(udocs-processor_module_cli)

set(MODULE_API_PROTO_DIR "${CMAKE_BINARY_DIR}/${RESOURCES_DIR}/res/misc/proto")
file(MAKE_DIRECTORY ${PROTO_GENERATED})
if(PLATFORM_WIN64)
    set(EXTENSION ".exe")
else()
    set(EXTENSION "")
endif()

execute_process(
        COMMAND "${CMAKE_BINARY_DIR}/execs/protoc${EXTENSION}"
            -I ${MODULE_API_PROTO_DIR}
            --plugin=protoc-gen-grpc=${CMAKE_BINARY_DIR}/execs/grpc_cpp_plugin${EXTENSION}
            --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_GENERATED}
            --grpc_out=${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_GENERATED}
            api/Status.proto
            api/products/Product.proto
)

add_library(${PROJECT_NAME} SHARED
        generated/api/Status.pb.h
        generated/api/Status.pb.cc
        generated/api/products/Product.grpc.pb.cc
        generated/api/products/Product.grpc.pb.h
        generated/api/products/Product.pb.cc
        generated/api/products/Product.pb.h
        include/udocs-processor/cli/SurDocsCLI.h
        cli/SurDocsCLI.cpp
        include/udocs-processor/cli/views/GenerateView.h
        include/udocs-processor/cli/views/InitView.h
        cli/views/FtxGenerateView.cpp
        include/udocs-processor/cli/views/FtxGenerateView.h
        cli/views/FtxInitView.cpp
        include/udocs-processor/cli/views/FtxInitView.h
        include/udocs-processor/cli/views/View.h
        cli/ue/UnrealInteraction.cpp
        include/udocs-processor/cli/ue/UnrealInteraction.h
        cli/commands/InitCommand.cpp
        include/udocs-processor/cli/commands/InitCommand.h
        cli/commands/GenerateCommand.cpp
        include/udocs-processor/cli/commands/GenerateCommand.h
        cli/commands/generate/CppPreparer.cpp
        include/udocs-processor/cli/commands/generate/CppPreparer.h
        cli/commands/generate/BpPreparer.cpp
        include/udocs-processor/cli/commands/generate/BpPreparer.h
        cli/commands/generate/Generator.cpp
        include/udocs-processor/cli/commands/generate/Generator.h
        cli/SurDocsCLI.cpp
        include/udocs-processor/cli/SurDocsCLI.h
        include/udocs-processor/cli/thirdparty/WinReg.hpp
        include/udocs-processor/cli/commands/generate/rendering/Templater.h
        include/udocs-processor/cli/commands/generate/rendering/UDocsDocumentRenderer.h
        cli/commands/generate/rendering/UDocsDocumentRenderer.cpp
        cli/commands/generate/rendering/Templater.cpp
        cli/cli/GenerateCLI.cpp
        include/udocs-processor/cli/cli/GenerateCLI.h
        cli/views/View.cpp
        cli/views/GenerateView.cpp
        cli/views/FtxView.cpp
        include/udocs-processor/cli/views/FtxView.h
        cli/CLISignalHandler.cpp
        include/udocs-processor/cli/CLISignalHandler.h
        cli/views/NonInteractiveGenerateView.cpp
        include/udocs-processor/cli/views/NonInteractiveGenerateView.h
        cli/surreal/SurrealLoader.cpp
        include/udocs-processor/cli/surreal/SurrealLoader.h
        cli/cli/InitCLI.cpp
        cli/version/VersionChecker.cpp
        cli/version/grpc.cpp
        cli/version/grpcProductService.cpp
        include/udocs-processor/cli/version/ProductService.h
        include/udocs-processor/cli/version/grpc.h
        include/udocs-processor/cli/version/grpcProductService.h
        include/udocs-processor/cli/version/VersionChecker.h)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "M-UDocs-CLI")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,$ORIGIN/")

set(UNIX_LIBRARIES "")

if(PLATFORM_UNIX)
    set(UNIX_LIBRARIES pthread)
endif()

target_link_libraries(${PROJECT_NAME}
        ${UNIX_LIBRARIES}
        udocs-processor_module_telemetry
        udocs-processor_module_processor
        udocs-processor_module_serializer
        udocs-processor_module_util
        CLI11::CLI11
        Boost::boost
        Boost::filesystem
        ftxui::ftxui)

target_include_directories(${PROJECT_NAME} PUBLIC include PUBLIC "generated")
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
