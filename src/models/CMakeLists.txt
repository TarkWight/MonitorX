# src/models/CMakeLists.txt
add_library(ModelsLib
    FileEntryModel.cpp
    FileEntryModel.hpp
)

target_include_directories(ModelsLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ModelsLib
    PUBLIC Qt6::Core
)

target_compile_features(ModelsLib
    PUBLIC cxx_std_23
)