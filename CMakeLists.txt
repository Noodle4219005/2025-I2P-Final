cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_Final VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(ALLEGRO_VSYNC 2) # Turn off vsync

include_directories(.)
include_directories("Map")
include_directories("res")

file(COPY "res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "Map" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "mania-config.ini" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(PROJECT_CPP_H_FILES 
    # Beatmap
    "Beatmap/BeatmapParser.h" 
    "Beatmap/BeatmapParser.cpp" 
    "Beatmap/DifficultyCounter.cpp" 
    "Beatmap/DifficultyCounter.h"
    "Engine/SingleThreadWorker.h"
    "Engine/SingleThreadWorker.cpp"

    # Engine
    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp

    # Objects
    "Objects/HitObject.h" 
    "Objects/HitObject.cpp"
    "Objects/Hold.h"
    "Objects/Hold.cpp"
    "Objects/Node.h"
    "Objects/Node.cpp"
     
    # Scene
    "Scene/Game.cpp" 
    "Scene/Game.h"
    "Scene/Menu.cpp" 
    "Scene/Menu.h"
    "Scene/RankingPanel.cpp" 
    "Scene/RankingPanel.h"
    "Scene/Start.cpp"
    "Scene/Start.h"

    # Skin
     
    "Skin/Skin.cpp" 
    "Skin/Skin.h" 

    # UI
    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp
    "UI/HUD.h" 
    "UI/HUD.cpp" 
    "UI/Component/BeatmapCard.h"
    "UI/Component/BeatmapCard.cpp"
    "UI/HitErrorGraph.cpp"
    "UI/HiterrorGraph.h"

    # util
    "util/GameData.h"
    "util/GameData.cpp"
    "util/ErrorCalculator.h" 
    "util/ErrorCalculator.cpp" 
    "util/Constant.h" 
    "util/Constant.cpp" 

    main.cpp
)

if (WIN32) 
    add_executable(2025_I2P2_Final WIN32 ${PROJECT_CPP_H_FILES})
else()
    add_executable(2025_I2P2_Final ${PROJECT_CPP_H_FILES})
endif()

find_package(allegro CONFIG REQUIRED)
target_link_libraries(2025_I2P2_Final Allegro::allegro Allegro::allegro_main Allegro::allegro_font Allegro::allegro_dialog 
                      Allegro::allegro_primitives Allegro::allegro_image Allegro::allegro_ttf 
                      Allegro::allegro_audio Allegro::allegro_acodec)
target_link_libraries(2025_I2P2_Final)

if(WIN32)
    target_link_libraries(2025_I2P2_Final winmm)
endif()

install(TARGETS 2025_I2P2_Final RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY Map DESTINATION bin)
install(DIRECTORY res DESTINATION bin)
install(FILES "mania-config.ini" "allegro_monolith-5.2.dll" DESTINATION bin)
# these command are from https://discourse.cmake.org/t/install-dlls-on-windows/6904 and gpt
install(CODE [[
    file(GET_RUNTIME_DEPENDENCIES EXECUTABLES "$<TARGET_FILE:2025_I2P2_Final>"
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        RESOLVED_DEPENDENCIES_VAR deps
        UNRESOLVED_DEPENDENCIES_VAR udeps
    )

    foreach(dep IN LISTS deps)
        file(INSTALL
            DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
            TYPE SHARED_LIBRARY
            FILES "${dep}"
        )
    endforeach()

    if(udeps)
        message(WARNING "Unresolved dependencies: ${udeps}")
    endif()
]])

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${2025_I2P2_Final_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${2025_I2P2_Final_VERSION_MINOR}")
set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)
