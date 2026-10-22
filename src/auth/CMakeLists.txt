add_library(hms_auth Auth.cpp)
target_link_libraries(hms_auth PUBLIC hms_common hms_security)
