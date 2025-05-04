# src/services/CMakeLists.txt
add_library(ServicesLib
    FileMonitorService.cpp
    FileMonitorService.hpp
)

target_include_directories(ServicesLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ServicesLib
    PUBLIC ConfigLib ModelsLib Qt6::Core Qt6::Concurrent
    PRIVATE ConfigLib UtilsLib
)

target_compile_features(ServicesLib
    PUBLIC cxx_std_23
)
