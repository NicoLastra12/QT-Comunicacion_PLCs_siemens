cmake_minimum_required(VERSION 3.19)
project(Prueba_signals_slots LANGUAGES CXX)

set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff <DEFINES> <INCLUDES> <FLAGS> -i <SOURCE> -o <OBJECT>")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

set(APP_RC "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources_app.rc")

qt_add_executable(Prueba_signals_slots
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    external/snap7/snap7.cpp
    resources.qrc
    ${APP_RC}
)

target_link_directories(Prueba_signals_slots
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/snap7
)

target_link_libraries(Prueba_signals_slots
    PRIVATE
        Qt::Core
        Qt::Widgets
        snap7
)


install(TARGETS Prueba_signals_slots
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Prueba_signals_slots
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
