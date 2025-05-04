# src/ui/CMakeLists.txt
qt_add_library(UiLib STATIC
MainWindow.hpp MainWindow.cpp MainWindow.ui
)

target_include_directories(UiLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(UiLib
    PUBLIC Qt6::Widgets
)

target_compile_features(UiLib
    PUBLIC cxx_std_23
)
