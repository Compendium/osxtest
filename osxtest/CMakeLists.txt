# CMakeLists.txt
project(gametest)

cmake_minimum_required(VERSION 2.8)
ADD_EXECUTABLE(gametestexec main.cpp CShader.cpp CTexture.cpp CTypes.cpp CVertexBatch.cpp CVertexBuffer.cpp) #todo add sources here?
if (UNIX)
  message(status "Setting GCC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -g -Wall")
else()
  message(status "Setting MSVC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc-")
endif()
message(status "** CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

include(FindPkgConfig) #todo check if pkg-config works on windows with sdl?
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
find_package(OpenGL)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2IMAGE_INCLUDE_DIRS})
include_directories(${SDL2TTF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(gametestexec ${SDL2_LIBRARIES})
TARGET_LINK_LIBRARIES(gametestexec ${SDL2IMAGE_LIBRARIES})
TARGET_LINK_LIBRARIES(gametestexec ${SDL2TTF_LIBRARIES})
include_directories(./glm)
TARGET_LINK_LIBRARIES(gametestexec ${OPENGL_LIBRARIES})

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
#todo cleanup!
