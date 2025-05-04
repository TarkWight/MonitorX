cmake_minimum_required(VERSION 3.16)
project(MonitorX VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (MSVC)
  add_compile_options(/utf-8)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Concurrent)

set(TS_FILES MonitorX_ru_RU.ts)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY   ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY   ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/ui)
add_subdirectory(src/app)

set(RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_custom_target(copy_config ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config/config.json
        ${RUNTIME_DIR}/config.json
)
add_dependencies(copy_config MonitorX)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${RUNTIME_DIR}/resources
)
add_dependencies(copy_resources MonitorX)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

include(GNUInstallDirs)
install(TARGETS MonitorX
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
