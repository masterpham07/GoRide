cmake_minimum_required(VERSION 3.21)

project(GoRide LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Svg
)

qt_add_executable(GoRide
    WIN32
    src/main.cpp

    src/models/Map.cpp
    src/models/User.cpp
    src/models/Place.cpp

    src/services/FareCalculator.cpp
    src/services/MatchingEngine.cpp
    src/services/PaymentService.cpp
    src/services/MapService.cpp
    src/services/PathFinder.cpp

    src/data/FileHandler.cpp

    src/ui/qt/MainWindow.cpp
    include/ui/qt/MainWindow.h

    src/ui/qt/MapWidget.cpp
    include/ui/qt/MapWidget.h

    include/models/Place.h

    resources/resources.qrc
)

target_include_directories(GoRide PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(GoRide PRIVATE
    Qt6::Widgets
    Qt6::Svg
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
