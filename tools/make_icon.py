"""
Name:        make_icon.py
Description: makes the port's icons from the picture of Raphael on a friend's wall (ports/sdl2/icon/raphael_mural.png),
             and the TV's banner from the four turtles on it (turtles_mural.png)
Version:     0.3.0
Created:     04-11-2026
Modified:    07-11-2026
Author:      Sebastian Toyos

Usage: python tools/make_icon.py
       python tools/make_icon.py --ico PICTURE ICO

The icon is Raphael's face from a friend's mural (Sebas: "es un dibujo de un mural de una pared de un amigo"),
nothing of the game's: the picture's square around the face, scaled to each icon the port has.
- ports/sdl2/icon: tmnt.png (1024 x 1024, its corners rounded, the computers' windows and Linux's), tmnt.ico (16 to
  256, Windows' executables) and tmnt.icns (the macOS .apps);
- ports/android/res: the launcher's icon, ic_launcher_art.png (288 x 288, the picture in the adaptive icon's 72 dp
  that the launcher shows, mipmap-anydpi-v26/ic_launcher.xml) and ic_launcher.png (192 x 192, the icon before
  Android 8);
- ports/switch/icon.jpg (256 x 256, the HOME menu's).
The TV's banner (Android TV, Google TV: the Chromecast with Google TV's apps) is the band of the same mural's four
turtles' faces, 16:9 (Sebas: "recorta de donde quieras"): ports/android/res/drawable-xhdpi/banner.png, 320 x 180.
A player's own picture still takes its place in his builds and in what the tools make (local/, TMNT APK, TMNT NSP).

--ico makes the Windows executable's icon of a picture of the player's own instead (CMakeLists.txt, with
local/icon_pc.png or TMNT_PC_ICON): square, the picture whole in the middle of it, see-through around.
"""
import sys
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parent.parent
SOURCE = ROOT / "ports" / "sdl2" / "icon" / "raphael_mural.png"
FACE = (10, 0, 170, 160)                 # the picture's square around the face (left, top, right, bottom)
OUT = ROOT / "ports" / "sdl2" / "icon"
ANDROID = ROOT / "ports" / "android" / "res" / "mipmap-xxxhdpi"
SWITCH = ROOT / "ports" / "switch" / "icon.jpg"
TURTLES = ROOT / "ports" / "sdl2" / "icon" / "turtles_mural.png"
TURTLES_BAND = (0, 196, 575, 519)        # the four faces' band, 16:9 (left, top, right, bottom)
BANNER = ROOT / "ports" / "android" / "res" / "drawable-xhdpi" / "banner.png"
BANNER_SIZE = (320, 180)                 # Android TV's banner, in xhdpi's pixels
NAME = "tmnt"
SIZE = 1024
CORNER = 180                             # the computers' icon's rounded corners, in its pixels
ICO_SIZES = [16, 24, 32, 48, 64, 128, 256]


def face(side):
    """The face, 'side' pixels square (RGB)."""
    return Image.open(SOURCE).convert("RGB").crop(FACE).resize((side, side), Image.LANCZOS)


def rounded(picture, radius):
    """'picture' with its corners rounded, see-through outside them (RGBA)."""
    scale = 4    # (the corners drawn larger and scaled down: smooth edges)
    mask = Image.new("L", (picture.width * scale, picture.height * scale), 0)
    ImageDraw.Draw(mask).rounded_rectangle((0, 0, mask.width - 1, mask.height - 1), radius * scale, fill=255)
    shaped = picture.convert("RGBA")
    shaped.putalpha(mask.resize(picture.size, Image.LANCZOS))
    return shaped


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    picture = rounded(face(SIZE), CORNER)
    picture.save(OUT / f"{NAME}.png")
    picture.save(OUT / f"{NAME}.ico", sizes=[(side, side) for side in ICO_SIZES])
    picture.save(OUT / f"{NAME}.icns")
    face(288).save(ANDROID / "ic_launcher_art.png")
    rounded(face(192), 192 * CORNER // SIZE).save(ANDROID / "ic_launcher.png")
    face(256).save(SWITCH, quality=95)
    banner()
    print(f"written: {NAME}.png, {NAME}.ico, {NAME}.icns in {OUT.relative_to(ROOT)}; ic_launcher_art.png and "
          f"ic_launcher.png in {ANDROID.relative_to(ROOT)}; {SWITCH.relative_to(ROOT)}; {BANNER.relative_to(ROOT)}")


def banner():
    """The TV's banner: the four turtles' faces."""
    BANNER.parent.mkdir(parents=True, exist_ok=True)
    picture = Image.open(TURTLES).convert("RGB").crop(TURTLES_BAND)
    picture.resize(BANNER_SIZE, Image.LANCZOS).save(BANNER, optimize=True)


def picture_ico(picture, ico):
    """An .ico of a picture of the player's own, its sizes the project's icon's."""
    image = Image.open(picture).convert("RGBA")
    side = max(image.size)
    square = Image.new("RGBA", (side, side), (0, 0, 0, 0))
    square.paste(image, ((side - image.width) // 2, (side - image.height) // 2))
    Path(ico).parent.mkdir(parents=True, exist_ok=True)
    square.resize((256, 256), Image.LANCZOS).save(ico, sizes=[(size, size) for size in ICO_SIZES])


if __name__ == "__main__":
    if len(sys.argv) == 4 and sys.argv[1] == "--ico":
        picture_ico(sys.argv[2], sys.argv[3])
    elif len(sys.argv) == 1:
        main()
    else:
        sys.exit("Usage: python tools/make_icon.py [--ico PICTURE ICO]")
