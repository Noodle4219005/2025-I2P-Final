cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_Final)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories("Map")
include_directories("res")
include_directories("res/skin")
include_directories("res/fonts")

file(COPY "res" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "Map" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2025_I2P2_Final
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
    "Engine/ImageHelper.cpp" 
    
    "Scene/Game.cpp" 
    "Scene/Game.h"

    "Beatmap/BeatmapParser.h" 
    "Beatmap/BeatmapParser.cpp" 

    "Skin/SkinBuilder.cpp" 
    "Skin/Skin.cpp" 
    "Skin/Skin.h" 
    "Skin/SkinImage.cpp" 
    "Skin/SkinImage.h" 
    "Skin/SkinConfig.h" 

    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp

    "util/GameData.h"
    "util/GameData.cpp"
    "Render/HUD.cpp" 
    "Objects/Hold.cpp"

    main.cpp
 "Objects/Node.h" "Objects/Hold.h" "Skin/SkinAudio.h" "Skin/SkinAudio.cpp" "Engine/ImageHelper.h" "util/Constants.h" "Skin/DummySkin.cpp" "util/ErrorCalculator.h" "util/ErrorCalculator.cpp")

find_package(allegro CONFIG REQUIRED)
target_link_libraries(2025_I2P2_Final Allegro::allegro Allegro::allegro_main Allegro::allegro_font Allegro::allegro_dialog 
                      Allegro::allegro_primitives Allegro::allegro_image Allegro::allegro_ttf 
                      Allegro::allegro_audio Allegro::allegro_acodec)
target_link_libraries(2025_I2P2_Final)
