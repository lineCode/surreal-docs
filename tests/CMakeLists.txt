#
#  Copyright © 2022, Medelfor, Limited. All rights reserved.
#  For any additional information refer to https://medelfor.com
#

project(udocs-processor_tests)
enable_testing()

set(UNIX_LIBRARIES "")

if(PLATFORM_UNIX)
    set(UNIX_LIBRARIES pthread)
endif()

add_executable(${PROJECT_NAME}
        SurrealMarkdownTests.cpp)

target_link_libraries(${PROJECT_NAME}
        udocs-processor_module_processor
        GTest::gtest
        ${UNIX_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "tests")

set(TESTS_INSTALL_TO ${CMAKE_INSTALL_PREFIX}${TESTS_INSTALL_POSTFIX})

install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION ${TESTS_INSTALL_TO})
