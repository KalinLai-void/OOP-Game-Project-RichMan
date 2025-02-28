# cmake_minimum_required(VERSION 3.10)
# project(MiniGameManagerTest)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS_DEBUG "/MDd") # 強制所有 Debug 版本使用 MDd (動態 Debug)
# set(CMAKE_CXX_FLAGS_RELEASE "/MD") # 強制所有 Release 版本使用 MD (動態 Release)

# # Google Test
# add_subdirectory(C:/Code/googletest googletest_build)
# include_directories(C:/Code/googletest/include)
# include_directories(C:/Code/monopoly/monopoly)

# # 測試程式
# add_executable(MiniGameManagerTest
#     C:/Code/monopoly/tests/MiniGameManagerTest.cpp
#     C:/Code/monopoly/monopoly/MiniGameManager.cpp
#     C:/Code/monopoly/monopoly/Player.cpp
# )

# target_link_libraries(MiniGameManagerTest gtest gtest_main)


cmake_minimum_required(VERSION 3.10)
project(MiniGameManagerTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 統一使用 MDd (動態 Debug) 和 MD (動態 Release)
set(CMAKE_CXX_FLAGS_DEBUG "/MDd")
set(CMAKE_CXX_FLAGS_RELEASE "/MD")

# Google Test
add_subdirectory(C:/Code/googletest googletest_build)

# 確保 Google Test 也使用 MDd
set_target_properties(gtest PROPERTIES COMPILE_FLAGS "/MDd")
set_target_properties(gtest_main PROPERTIES COMPILE_FLAGS "/MDd")

include_directories(C:/Code/googletest/include)
include_directories(C:/Code/monopoly/monopoly)

# 測試程式
add_executable(MiniGameManagerTest
    C:/Code/monopoly/tests/MiniGameManagerTest.cpp
    C:/Code/monopoly/monopoly/MiniGameManager.cpp
    C:/Code/monopoly/monopoly/Player.cpp
)

target_link_libraries(MiniGameManagerTest gtest gtest_main)
