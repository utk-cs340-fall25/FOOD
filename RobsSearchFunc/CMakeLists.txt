cmake_minimum_required(VERSION 3.5)

project(RecipeApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
)

set(HEADERS
    mainwindow.h
    ui_mainwindow.h
)

# Explicitly run moc on headers with Q_OBJECT
qt6_wrap_cpp(MOC_FILES mainwindow.h)

# Create executable including generated moc files
add_executable(RecipeApp
    ${SOURCES}
    ${HEADERS}
    ${MOC_FILES}  # <- Add moc files here
)

target_link_libraries(RecipeApp PRIVATE Qt6::Widgets)
