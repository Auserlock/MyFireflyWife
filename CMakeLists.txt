cmake_minimum_required(VERSION 3.25)
project(MyWifeFirefly VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SFML "${CMAKE_SOURCE_DIR}/lib/SFML-2.6.1")
set(CMAKE_PREFIX_PATH "${SFML}")
file(GLOB PROJECT_SOURCES
        "src/*.cpp"
        "src/MediaPlayer/*.cpp"
)
file(GLOB PROJECT_INCLUDE
    "include/*.h"
)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(SFML 2.6.1 COMPONENTS system audio REQUIRED)

add_executable(MyWifeFirefly
        ${PROJECT_SOURCES}
        ${PROJECT_INCLUDE}
        src/Window/Firefly/FireflyWindowConfig.cpp
)

target_include_directories(MyWifeFirefly PRIVATE include ${SFML}/include)
target_link_libraries(MyWifeFirefly
        PRIVATE Qt::Core
        Qt::Gui
        Qt::Widgets
        sfml-audio
        sfml-system
)

