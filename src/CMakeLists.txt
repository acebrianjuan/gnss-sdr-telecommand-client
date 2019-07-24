find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)
if(NOT Qt5_FOUND)
     message(FATAL_ERROR "Fatal error: Qt5 required.")
endif(NOT Qt5_FOUND)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT5_LIBRARIES
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
)

set(TARGET ${CMAKE_PROJECT_NAME})

set(SOURCES
    main.cpp
    mainwindow.cpp
    telnetmanager.cpp
)

set(UI_SOURCES
    mainwindow.ui
)

set(RESOURCES
    resources.qrc
)

add_executable(${TARGET} ${SOURCES} ${UI_SOURCES} ${RESOURCES})

target_link_libraries(${TARGET} PUBLIC ${QT5_LIBRARIES})
