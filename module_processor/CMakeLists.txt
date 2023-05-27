#
#  Copyright © 2022, Medelfor, Limited. All rights reserved.
#  For any additional information refer to https://medelfor.com
#

project(udocs-processor_module_processor)

add_library(${PROJECT_NAME} SHARED
        include/udocs-processor/UDocsProcessor.h
        UDocsProcessor.cpp
        include/udocs-processor/treeloaders/TreeLoader.h
        treeloaders/UDataTreeLoader.cpp
        include/udocs-processor/treeloaders/UDataTreeLoader.h
        treeloaders/DoxybookTreeLoader.cpp
        include/udocs-processor/treeloaders/DoxybookTreeLoader.h
        treeloaders/TreeLoader.cpp
        treeloaders/DoxybookDataLoader.cpp
        include/udocs-processor/treeloaders/DoxybookDataLoader.h
        include/udocs-processor/subprocessors/Subprocessor.h
        include/udocs-processor/subprocessors/ComplementFinder.h
        subprocessors/Subprocessor.cpp
        subprocessors/CrossReferencer.cpp
        subprocessors/ComplementFinder.cpp
        include/udocs-processor/subprocessors/CrossReferencer.h
        subprocessors/ReferenceCacheBuilder.cpp
        include/udocs-processor/subprocessors/ReferenceCacheBuilder.h
        treeloaders/DocCommentProcessor.cpp
        include/udocs-processor/treeloaders/DocCommentProcessor.h
        LocationBuilder.cpp
        include/udocs-processor/LocationBuilder.h
        include/udocs-processor/treeloaders/PagesTreeLoader.h
        treeloaders/PagesTreeLoader.cpp
        subprocessors/DocPathBuilder.cpp
        include/udocs-processor/subprocessors/DocPathBuilder.h
        include/udocs-processor/subprocessors/ExportPolicy.h
        subprocessors/ExportPolicy.cpp
        include/udocs-processor/NodeHasher.h
        NodeHasher.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "M-UDocs-Processor")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,$ORIGIN/")

set(UNIX_LIBRARIES "")

if(PLATFORM_UNIX)
    set(UNIX_LIBRARIES pthread)
endif()

target_link_libraries(${PROJECT_NAME}
        ${UNIX_LIBRARIES}
        udocs-processor_module_serializer
        udocs-processor_module_base
        udocs-processor_module_report
        udocs-processor_module_util
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        Crc32c::Crc32c
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
