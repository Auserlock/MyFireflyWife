cmake_minimum_required(VERSION 3.25)
project(MyWifeFirefly VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 库文件搜索位置
set(LIBRARY_PATH "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_PREFIX_PATH "${LIBRARY_PATH}")

set(SFML "${LIBRARY_PATH}/SFML-2.61/")
set(spdlog "${LIBRARY_PATH}/spdlog-1.14.1/")

file(GLOB PROJECT_SOURCES
        "src/*.cpp"
        "src/MediaPlayer/*.cpp"
        "src/Window/Firefly/*.cpp"
        "src/Log/*.cpp"
        "src/ActionEvent/*.cpp"
)
file(GLOB PROJECT_INCLUDE
    "include/*.h"
)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(SFML 2.6.1 COMPONENTS audio REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(spdlog 1.14.1 REQUIRED)

add_executable(MyWifeFirefly
        ${PROJECT_SOURCES}
        ${PROJECT_INCLUDE}
        src/FireflyVoice/FireflyVoice.cpp
        include/FireflyVoice.h
)

target_include_directories(MyWifeFirefly
        PRIVATE
        include
        ${SFML}/include
        ${spdlog}/include)
target_link_libraries(MyWifeFirefly
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        sfml-audio
        nlohmann_json::nlohmann_json
        spdlog::spdlog
)

set_target_properties(MyWifeFirefly PROPERTIES LINK_FLAGS "-Wl,-rpath,./lib")

# 安装路径
install(TARGETS MyWifeFirefly DESTINATION ./)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/config/main.json DESTINATION data/config/)

include(CPack)

set(CPACK_PACKAGE_NAME "MyWifeFirefly")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_DESCRIPTION "应用程序打包Test")
set(CPACK_PACKAGE_VENDOR "Auserlock")

