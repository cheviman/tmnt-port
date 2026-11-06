#!/bin/sh
# TMNT for Anbernic's H700 handhelds (stock firmware): the console version in the folder tmnt beside this script,
# with the firmware's SDL2; what it says goes to tmnt/tmnt.log
HERE=$(cd "$(dirname "$0")" && pwd)
cd "$HERE/tmnt" || exit 1
chmod +x ./TMNT 2>/dev/null
./TMNT > tmnt.log 2>&1
sync
