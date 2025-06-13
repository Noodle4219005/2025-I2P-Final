cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_Final)

set(CMAKE_CXX_STANDARD 20)
set(ALLEGRO_VSYNC 2) # Turn off vsync

include_directories(.)
include_directories("Map")
include_directories("res")

file(COPY "res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "Map" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "mania-config.ini" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2025_I2P2_Final WIN32
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

find_package(allegro CONFIG REQUIRED)
target_link_libraries(2025_I2P2_Final Allegro::allegro Allegro::allegro_main Allegro::allegro_font Allegro::allegro_dialog 
                      Allegro::allegro_primitives Allegro::allegro_image Allegro::allegro_ttf 
                      Allegro::allegro_audio Allegro::allegro_acodec)
target_link_libraries(2025_I2P2_Final)

if(WIN32)
    target_link_libraries(2025_I2P2_Final winmm)
endif()
