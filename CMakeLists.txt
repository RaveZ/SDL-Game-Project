cmake_minimum_required (VERSION 3.8)
project ("SDL_Game" C CXX)  # Enable C and C++ languages

find_package(SDL3 REQUIRED)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Add source to this project's executable.
add_executable (SDL_Game "SDL_Game.cpp" "SDL_Game.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SDL_Game PROPERTY CXX_STANDARD 20)
endif()

# Link SDL3 libraries
target_link_libraries(SDL_Game PRIVATE SDL3::SDL3)

# TODO: Add tests and install targets if needed.