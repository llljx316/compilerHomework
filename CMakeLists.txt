cmake_minimum_required(VERSION 3.5)

project(compiler_11 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
#        include/mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(compiler_11
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        main.cpp mainwindow.cpp
        include/KeyWord.h include/lexicalanalyzer.h include/lritem.h include/mainwindow.h include/midcodeparser.h include/parser.h include/production.h include/scanner.h include/token.h include/TokenType.h include/widget.h
        src/lexicalanalyzer.cpp src/lritem.cpp src/midcodeparser.cpp src/parser.cpp src/production.cpp src/scanner.cpp src/token.cpp src/widget.cpp
        src/mainpage.cpp
        include/mainpage.h
        src/util.cpp
        include/util.h
        src/QuadExpression.cpp
        include/QuadExpression.h
        include/Lexeme.h
        src/Lexeme.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET compiler_11 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(compiler_11 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(compiler_11
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(compiler_11 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.compiler_11)
endif()
set_target_properties(compiler_11 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS compiler_11
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include_directories(./include)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(compiler_11)
endif()



