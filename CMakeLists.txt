cmake_minimum_required(VERSION 3.8)
project("SDL_Game" C CXX)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(SDL_Game "SDL_Game.cpp" "SDL_Game.h")

if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET SDL_Game PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(SDL_Game PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

# --- Asset handling ---

# 1) Copy data/ next to the exe after each build
add_custom_command(TARGET SDL_Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:SDL_Game>/data"
    COMMENT "Copying data/ next to executable"
)

# 2) Run from the exe's directory so relative paths resolve
set_property(TARGET SDL_Game PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SDL_Game>")