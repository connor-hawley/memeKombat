cmake_minimum_required(VERSION 3.6)
project(memeKombat)

#define EXECUTABLE NAME HERE
set(EXECUTABLE_NAME "memeKombat")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})

set(SOURCE_FILES
        main.cpp
        Game.cpp Game.h GameObject.h RegularGameObject.h RegularGameObject.cpp AnimatedGameObject.cpp AnimatedGameObject.h GameObject.cpp)

#copy file to where my executable is
file(COPY "content/memeCombat.jpg" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/pressEntertoBegin.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/chooseFighter.jpg" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/comicSans.ttf" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/blackScreen.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/datBoi64.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/evilKermit64.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/harambe64.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/joeBiden64.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/joeBidenSprites.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/kangaroo64.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/pepe64.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/spongegar64.png" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "content/theCourtyard.jpg" DESTINATION "${CMAKE_BINARY_DIR}")





add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})