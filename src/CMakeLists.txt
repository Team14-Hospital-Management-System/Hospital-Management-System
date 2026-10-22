# Shared file: set up once, then not edited. Each module owner edits ONLY their own src/<module>/CMakeLists.txt.
add_subdirectory(security)
add_subdirectory(storage)
add_subdirectory(auth)
add_subdirectory(patient)
add_subdirectory(doctor)
add_subdirectory(appointment)
add_subdirectory(billing)
add_subdirectory(report)
add_subdirectory(cli)

add_executable(hms main.cpp)
target_link_libraries(hms PRIVATE hms_cli)
