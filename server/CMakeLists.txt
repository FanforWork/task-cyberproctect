add_executable(process_monitor
    ${PROJECT_SOURCE_DIR}/main.cpp
    server.cpp
)
target_include_directories(process_monitor PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/server
)
target_compile_definitions(process_monitor PRIVATE BUILD_SERVER)
install(TARGETS process_monitor DESTINATION /usr/local/bin)
install(FILES process_monitor.service DESTINATION /etc/systemd/system)