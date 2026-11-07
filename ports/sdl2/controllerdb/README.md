# SDL_GameControllerDB

`gamecontrollerdb.txt` is [SDL_GameControllerDB](https://github.com/mdqinc/SDL_GameControllerDB)'s file at commit
`07af1d50187e94c861c9f9f5d8a55b148f11306b` (07-11-2026), as it is, with its `LICENSE` (zlib License): the community's
mappings of game controllers, which SDL does not know of all by itself (a player's report: on an Nvidia Shield a PS4
controller did not move the cursor, and in a game neither it nor an Xbox controller moved the turtle).

The window port's games on the computers and Android build their platform's lines into the executable
(`embed.cmake`, CMakeLists.txt's `tmnt_controller_db`), and `ports/sdl2/main.c` adds them to SDL as it starts. The
Switch and the Anbernic keep their own controls. To take a newer one: the same file from the repository's master,
and its commit here.
