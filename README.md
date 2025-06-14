# Final - Osu! Mania in Allegro5

This is a final project of the course "Introduction to Programming II" from National TsingHua University. It mainly focuses on the osu mania feature and is based on allegro5 game library.

Reference: McOsu, Osu!Lazer Mania, Quaver, rosu-pp, others (You can see them in my source code.)

## Demo
[![2025_I2P2_Final-Demo_Video](https://img.youtube.com/vi/m0Nm9fBc67s/0.jpg)](https://www.youtube.com/watch?v=m0Nm9fBc67s)

## Game feature

- Parse and play Osu mania beatmap.
- An Osu mania difficulty counter copy.
- ACC, score, hiterror bar displays in HUD.
- Auto Synchronized music playing and hitobject displacement
- SV(slider velocity) map is available
- Three mode: HD(Hidden), DT(Double Time), NF(No Failed) available
- Menu Multi-threading loading music
- Customized setting file
- Restricted skin file modification

## How to play?
1. Put the `.osz` file of a map you want into the `Map` foloder and unzip it(using 7-zip or others).
2. Reopen the game by clicking the .exe file
3. you can always modify mania-config.ini file to change game data, or modify the file in `./res/skin folder` to change the picture in skin before the game executed
4. Then enjoy your game.

### Keymap

- alt + scroll can change the volume
- both scrolling and key up/down can switch the songs.
- Here I list out some frequently used keys(for others, you can check out util/constant.cpp)

| Game Mode              | Keys          |
| ---------------------- | ------------- |
| 1 Key                  | [SPACE]       |
| 4 Keys                 | DFJK          |
| 5 Keys                 | DF[SPACE]JK   |
| 6 Keys                 | SDFJKL        |
| 7 Keys                 | SDF[SPACE]JKL |
| 10 Keys                | ASDFGHJKL;    |
| Toggle Auto Mod        | A             |
| Toggle Double Time Mod | S             |
| Toggle No Fail Mod     | D             |


## Build and Run
> If you want the console to display, please remove WIN32 in add_executable(2025_I2P2_Final WIN32 ... and rebuild with cmake : )

### Dependicies

- git and cmake(>=3.21)
- for windows, you need to install ninja and visual studio runtime first
- install vcpkg by Microsoft(https://learn.microsoft.com/zh-tw/vcpkg/get_started/get-started?pivots=shell-powershell)
2. Then run the following commands.
```shell
git clone https://github.com/Noodle4219005/2025-I2P-Final.git
cd 2025-I2P-Final
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build build
# for windows
./build/2025_I2P2_Final.exe
# for linux and mac
./build/2025_I2P2_Final.out
```
## TODO
- [x] hiterror graph in result scene
- [ ] Local scoreboard.
- [ ] Osu sound effect feature.
- [ ] Game scene can read osu hitobject files.
- [ ] Animation effect of the game.
