# Final - Osu! Mania in Allegro5

This is a final project of the course "Introduction to Programming II" from National TsingHua University. It mainly focuses on the osu mania feature and is based on allegro5 game library.

Reference: McOsu, Osu!Lazer Mania, Quaver, rosu-pp, others (You can see them in my source code.)

## Demo

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

> If you want the console to display, please remove WIN32 in add_executable(2025_I2P2_Final WIN32 ... and rebuild with cmake : )
## TODO
- [x] hiterror graph in result scene
- [ ] Local scoreboard.
- [ ] Osu sound effect feature.
- [ ] Game scene can read osu hitobject files.
- [ ] Animation effect of the game.
