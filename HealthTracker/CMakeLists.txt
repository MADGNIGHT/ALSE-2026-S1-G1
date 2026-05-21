cmake_minimum_required(VERSION 3.16)

project(HealthTracker LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Widgets)

add_executable(HealthTracker
    main.cpp

    db/DatabaseManager.h
    db/DatabaseManager.cpp
    db/UserDAO.h
    db/UserDAO.cpp
    db/MetricDAO.h
    db/MetricDAO.cpp

    logic/stats.h
    logic/stats_code.cpp
    logic/cuentas.h
    logic/cuentas_code.cpp

    ui/LoginWindow.h
    ui/LoginWindow.cpp
    ui/MainWindow.h
    ui/MainWindow.cpp
    ui/MetricInputDialog.h
    ui/MetricInputDialog.cpp
    ui/ChartsWidget.h
    ui/ChartsWidget.cpp
)

target_link_libraries(HealthTracker
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Widgets
)

include(GNUInstallDirs)

install(TARGETS HealthTracker
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
