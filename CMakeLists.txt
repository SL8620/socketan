# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(SocketCanDriverProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(test_socketcan
    socketcan_driver.cpp
    test_socketcan.cpp
)

# Include directories (if needed, add more)
target_include_directories(test_socketcan PRIVATE .)

# Link libraries (SocketCAN uses standard Linux libs, no extra needed usually)
# If real-time features require, link rt: target_link_libraries(test_socketcan rt)
target_link_libraries(test_socketcan pthread)  # For threads