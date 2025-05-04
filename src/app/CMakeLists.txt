# src/app/CMakeLists.txt
add_executable(MonitorX
    main.cpp
)

target_link_libraries(MonitorX
    PRIVATE
        UiLib
        Qt6::Widgets
)
