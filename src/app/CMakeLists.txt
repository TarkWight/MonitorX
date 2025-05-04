# src/app/CMakeLists.txt
add_executable(MonitorX
    main.cpp
)

target_link_libraries(MonitorX
    PRIVATE
        ConfigLib
        UiLib
        Qt6::Widgets
)
