cmake_minimum_required(VERSION 3.2)
project(OpenMapsTileDownloader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread ")

set(SOURCE_FILES src/main.cpp)
add_executable(OpenMapsTileDownloader ${SOURCE_FILES}
src/defines.h

src/TaskPool/Task.h
src/TaskPool/Pool.h
src/TaskPool/Pool.cpp

src/TaskPool/Utilities/Mutex.h
src/TaskPool/Utilities/Mutex.cpp

src/TaskPool/Utilities/ContitionVariable.h
src/TaskPool/Utilities/ContitionVariable.cpp

src/OpenMaps/OpenMapsDownload.h
src/OpenMaps/OpenMapsDownload.cpp)
