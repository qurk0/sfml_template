include(FetchContent)

FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.89.9
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG master
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(ImGui-SFML)

# all source files
set(SRC SoundStage.cpp)

# define the sound-stage target
sfml_add_example(sound-stage GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Audio SFML::Graphics ImGui-SFML::ImGui-SFML
                 RESOURCES_DIR resources)

# external dependency headers
target_include_directories(sound-stage SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/sound_stage)
