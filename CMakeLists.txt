 cmake_minimum_required(VERSION 3.5)
 project(ProjectGame)

 find_package(PkgConfig REQUIRED)
 pkg_check_modules(RAYLIB REQUIRED raylib)

 include_directories(${RAYLIB_INCLUDE_DIRS})
 link_directories(${RAYLIB_LIBRARY_DIRS})

 set(CMAKE_CXX_STANDARD 11)
 set(CMAKE_CXX_STANDARD_REQUIRED ON)


 add_executable(Game Game.cpp
         globals.h)

 target_link_libraries(Game ${RAYLIB_LIBRARIES})


