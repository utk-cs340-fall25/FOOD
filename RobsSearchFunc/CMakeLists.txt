cmake_minimum_required(VERSION 3.5)

project(RecipeApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt (Qt6 version)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Define sources
set(SOURCES
    main.cpp
    mainwindow.cpp
)

set(HEADERS
    mainwindow.h
)

set(UI_FILES
    mainwindow.ui
)

# Create the executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# Link with Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# --- Auto-copy recipes.txt to build folder ---
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/recipes.txt
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
