#!/bin/sh
# TMNT Arcade for Anbernic's H700 handhelds (stock firmware): the arcade game in the folder tmnt beside this script,
# with the firmware's SDL2; what it says goes to tmnt/tmnt-arcade.log
HERE=$(cd "$(dirname "$0")" && pwd)
cd "$HERE/tmnt" || exit 1
chmod +x "./TMNT Arcade" 2>/dev/null
"./TMNT Arcade" > tmnt-arcade.log 2>&1
sync
