add_executable(client_app
    ${PROJECT_SOURCE_DIR}/main.cpp
    client.cpp
)
target_include_directories(client_app PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/client
)
target_compile_definitions(client_app PRIVATE BUILD_CLIENT)
install(TARGETS client_app DESTINATION /usr/local/bin)