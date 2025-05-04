# src/utils/CMakeLists.txt
add_library(UtilsLib
    BackupManager.cpp
    BackupManager.hpp
    LogManager.cpp
    LogManager.hpp
)

target_include_directories(UtilsLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(UtilsLib
    PUBLIC Qt6::Core Qt6::Concurrent
)

target_compile_features(UtilsLib
    PUBLIC cxx_std_23
)
