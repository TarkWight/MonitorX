# src/config/CMakeLists.txt
add_library(ConfigLib
    ConfigManager.cpp
    ConfigManager.hpp
    IConfigManager.hpp
)

target_include_directories(ConfigLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ConfigLib
    PUBLIC Qt6::Core
    PRIVATE UtilsLib
)

target_compile_features(ConfigLib
    PUBLIC cxx_std_23
)
