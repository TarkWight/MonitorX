# src/di/CMakeLists.txt
add_library(DILib
    DependencyInjection.hpp
    DependencyInjection.cpp
)

target_include_directories(DILib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(DILib
    PUBLIC Qt6::Core
    PRIVATE
        ConfigLib
        ModelsLib
        ServicesLib
        UtilsLib
        ViewModelsLib
        UiLib
)

target_compile_features(DILib
    PUBLIC cxx_std_23
)
