cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(qmlbind20
  VERSION 0.1.0
  DESCRIPTION "C++ to QML bindings inspired by pybind11"
  HOMEPAGE_URL "io.whs31.qmlbind20"
  LANGUAGES C CXX
)

option(INTEGRATION_TESTS "Enable integration tests" OFF)
option(DEV_BINARY "Enable building of dev binary" OFF)

include(dependencies/cmake/all.cmake)
enable_cross_compile_options()

set_language_standard(20)
check_language_features()
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(leaf REQUIRED CONFIG)
find_package(QT NAMES Qt5 Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}
  COMPONENTS
    Core
    Quick
  REQUIRED
)

message(STATUS "[${PROJECT_NAME}] using qt version: qt ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}.")

add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::qmlbind20 ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/qmlbind20/qmlbind20.h> $<INSTALL_INTERFACE:include/qmlbind20/qmlbind20.h>

  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/qmlbind20.c++
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/module.c++
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++/component.c++
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    leaf::leaf
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/c++
)

target_add_metadata_definitions(${PROJECT_NAME} COMPANY "whs31")
enable_static_crt()

if(INTEGRATION_TESTS)
  target_add_integration_tests(${PROJECT_NAME}
    TESTS
      tests/test-main.c++
      tests/qrc.qrc
  )
endif()

if(DEV_BINARY)
  add_subdirectory(bin)
endif()

target_install_library(${PROJECT_NAME} NAME qmlbind20 NAMESPACE qmlbind20)

unset(INTEGRATION_TESTS CACHE)
unset(DEV_BINARY CACHE)
