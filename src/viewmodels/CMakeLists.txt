# src/viewmodels/CMakeLists.txt
add_library(ViewModelsLib
    MonitorViewModel.cpp
    MonitorViewModel.hpp
    IMonitorViewModel.hpp
)

target_include_directories(ViewModelsLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ViewModelsLib
    PUBLIC ServicesLib UtilsLib ConfigLib ModelsLib Qt6::Core Qt6::Gui
)

target_compile_features(ViewModelsLib
    PUBLIC cxx_std_23
)
