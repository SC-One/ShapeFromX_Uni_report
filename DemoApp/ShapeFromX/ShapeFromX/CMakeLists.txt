cmake_minimum_required(VERSION 3.14)

project(ShapeFromX VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick LinguistTools)

set(CV_DIR "C:/opencv/build" CACHE STRING "Please update this path to point to the opencv cmakes path.(in cmake variables you can too)")

option(TEST_PRJ_BEFORE_START "just for testing that opencv linked and it's ok." OFF)

if(${TEST_PRJ_BEFORE_START})
  add_compile_definitions(TEST_OPENCV_BY_US)
endif()
set(OpenCV_DIR ${CV_DIR}) # download from: https://github.com/opencv/opencv/releases , we tested: tag/4.8.1
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(TS_FILES ShapeFromX_fa_IR.ts)

set(PROJECT_SOURCES
  main.cpp
  qml.qrc
  ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(ShapeFromX
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
  )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ShapeFromX APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
  if(ANDROID)
    add_library(ShapeFromX SHARED
      ${PROJECT_SOURCES}
    )
  # Define properties for Android with Qt 5 after find_package() calls as:
  #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
  add_executable(ShapeFromX
    ${PROJECT_SOURCES}
    Utilities.h Utilities.cpp
  )
endif()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(ShapeFromX
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ShapeFromX)
endif()
set_target_properties(ShapeFromX PROPERTIES
  ${BUNDLE_ID_OPTION}
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
#install(TARGETS ShapeFromX
#  BUNDLE DESTINATION .
#  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
  qt_import_qml_plugins(ShapeFromX)
  qt_finalize_executable(ShapeFromX)
endif()

target_link_libraries(ShapeFromX PUBLIC ${OpenCV_LIBS})
