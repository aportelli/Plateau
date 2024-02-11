cmake_minimum_required(VERSION 3.5)

project(
  Plateau
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/dependencies/prefix/)

find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network PrintSupport
                                                       SvgWidgets Widgets Core5Compat)
find_package(LatAnalyze REQUIRED)

set(PROJECT_SOURCES
    src/Global.h
    src/main.cpp
    src/mousecmn.h
    src/MainWindow.h
    src/MainWindow.cpp
    src/MainWindow.ui
    src/data/CorrelatorData.h
    src/data/CorrelatorData.cpp
    src/data/DataOptionDialog.h
    src/data/DataOptionDialog.cpp
    src/data/DataOptionDialog.ui
    src/data/DataTableModel.h
    src/data/DataTableModel.cpp
    src/gnuplot/GnuplotWidget.h
    src/gnuplot/GnuplotWidget.cpp
    src/gnuplot/QtGnuplotEvent.h
    src/gnuplot/QtGnuplotEvent.cpp
    src/gnuplot/QtGnuplotInstance.h
    src/gnuplot/QtGnuplotInstance.cpp
    src/gnuplot/QtGnuplotItems.h
    src/gnuplot/QtGnuplotItems.cpp
    src/gnuplot/QtGnuplotScene.h
    src/gnuplot/QtGnuplotScene.cpp
    src/gnuplot/QtGnuplotWidget.h
    src/gnuplot/QtGnuplotWidget.cpp
    src/gnuplot/GnuplotWidget.ui)

qt_add_executable(Plateau MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_include_directories(Plateau PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(
  Plateau
  PRIVATE LatAnalyze::LatAnalyze Qt${QT_VERSION_MAJOR}::Core5Compat Qt${QT_VERSION_MAJOR}::Network
          Qt${QT_VERSION_MAJOR}::PrintSupport Qt${QT_VERSION_MAJOR}::SvgWidgets
          Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1. If
# you are developing for iOS or macOS you should consider setting an explicit,
# fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Plateau)
endif()
set_target_properties(
  Plateau
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

include(GNUInstallDirs)
install(
  TARGETS Plateau
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(Plateau)
endif()
