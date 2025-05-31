cmake_minimum_required(VERSION 3.16)

project(Calculator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Qml)

set(TS_FILES Calculator_ja_JP.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Calculator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Calculator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Calculator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Calculator
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Calculator PRIVATE Qt6::Widgets Qt6::Qml)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Calculator)
endif()
set_target_properties(Calculator PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Calculator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Calculator)
endif()

include(InstallRequiredSystemLibraries)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)

# QtのDLLとプラグインを自動でインストール先にコピー
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/QtDeploy.cmake OPTIONAL)

set(CPACK_GENERATOR "WIX")
set(CPACK_PACKAGE_NAME "Calculator")
set(CPACK_PACKAGE_VENDOR "YourName")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "関数電卓アプリ")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_WIX_UPGRADE_GUID "16c15f00-997e-479c-89ff-2cd30e55afe9") # 新規GUIDを生成して入れる
set(CPACK_PROJECT_NAME "Calculator")
include(CPack)

# Qt DLLをインストーラに含める（windeployqtでコピーされたDLLをinstallコマンドで追加）
if(WIN32)
    # Debug/Release両対応でQt DLL・プラグインをインストーラに含める
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Debug/" DESTINATION "."
        FILES_MATCHING PATTERN "Qt6*.dll"
        PATTERN "*.pdb" EXCLUDE
        PATTERN "*.lib" EXCLUDE
        PATTERN "*.exp" EXCLUDE
        PATTERN "*.obj" EXCLUDE
        PATTERN "*.ilk" EXCLUDE
        PATTERN "*.tlog" EXCLUDE
        PATTERN "*.recipe" EXCLUDE
        PATTERN "*.manifest" EXCLUDE
        PATTERN "*.exe" EXCLUDE
    )
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Debug/platforms" DESTINATION "platforms" OPTIONAL)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Debug/styles" DESTINATION "styles" OPTIONAL)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Debug/imageformats" DESTINATION "imageformats" OPTIONAL)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Debug/iconengines" DESTINATION "iconengines" OPTIONAL)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Debug/qml" DESTINATION "qml" OPTIONAL)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Debug/translations" DESTINATION "translations" OPTIONAL)
endif()
