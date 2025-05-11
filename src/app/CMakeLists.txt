# src/app/CMakeLists.txt
add_executable(MonitorX
    main.cpp
)

target_link_libraries(MonitorX
    PRIVATE
        ConfigLib
        ModelsLib
        ServicesLib
        UtilsLib
        ViewModelsLib
        UiLib
        DILib
        Qt6::Widgets
)
