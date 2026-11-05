"""
Name:        make_icon.py
Description: makes the port's icon: a manhole's lid, lit from below in green, on a plate in a night's colours
Version:     0.1.0
Created:     04-11-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

Usage: python tools/make_icon.py
       python tools/make_icon.py --ico PICTURE ICO

As outrunners-port's: the icon of the computers' executables (ports/sdl2/icon: tmnt.png, 1024 x 1024, tmnt.ico
for Windows, tmnt.icns for macOS). An original picture, drawn here from shapes alone: nothing of the game's art,
no character and no logo. A plain street's iron lid on its plate, the light of the sewer coming up around it.
Everything is drawn at twice the size and scaled down, so the edges are smooth.

--ico makes the Windows executable's icon of a picture of the player's own instead (CMakeLists.txt, with
local/icon_pc.png or TMNT_PC_ICON): square, the picture whole in the middle of it, see-through around.
"""
import sys
from pathlib import Path

import numpy as np
from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "ports" / "sdl2" / "icon"
NAME = "tmnt"
SIZE = 1024
SCALE = 2                       # (drawn at 2048, then scaled down)
ICO_SIZES = [16, 24, 32, 48, 64, 128, 256]


def rgb(red, green, blue):
    """A colour, as the drawing's arrays take it."""
    return np.array([red, green, blue], np.float32)


PLATE_MARGIN, PLATE_RADIUS = 48, 210     # the plate's room around it and its corners, in the icon's pixels
PLATE_TOP = rgb(0x14, 0x1C, 0x38)        # a night's blue at the top, to the sewer's green at the bottom
PLATE_BOTTOM = rgb(0x0E, 0x45, 0x2A)
GLOW = rgb(0x9A, 0xFF, 0x62)             # the light coming up around the lid
GLOW_REACH = 70                          # how far it goes out from the lid's edge (it falls to a third)

LID_X, LID_Y = 512, 512                  # the lid: its middle, its radius and the rim's inner edge
LID_RADIUS, RIM_INNER = 318, 270
HUB_RADIUS = 96                          # the plain disc in its middle
IRON = rgb(0x55, 0x5E, 0x6A)             # the rim and the hub
TILE = rgb(0x4A, 0x53, 0x5F)             # the raised squares of the lid's face
GROOVE = rgb(0x1E, 0x24, 0x2C)           # the grooves between them
PITCH, GROOVE_WIDTH, BEVEL = 56, 11, 6   # the squares' grid, in the icon's pixels
LIGHT = np.array([-0.6, -0.8], np.float32)    # where the light on the iron comes from: the top left


def coverage(distance, pixel):
    """How much of a pixel a shape covers, from its signed distance to the edge (negative inside)."""
    return np.clip(0.5 - distance / pixel, 0.0, 1.0)


def rounded_square(x, y, left, top, right, bottom, radius):
    """Signed distance to a square with rounded corners."""
    cx, cy = (left + right) / 2, (top + bottom) / 2
    qx = np.abs(x - cx) - ((right - left) / 2 - radius)
    qy = np.abs(y - cy) - ((bottom - top) / 2 - radius)
    outside = np.hypot(np.maximum(qx, 0), np.maximum(qy, 0))
    return outside + np.minimum(np.maximum(qx, qy), 0) - radius


def blend(under, over, amount):
    """'over' on 'under', 'amount' of it (0 to 1, per pixel)."""
    return under * (1 - amount[..., None]) + over * amount[..., None]


def lid_face(dx, dy):
    """The lid's face inside the rim: raised squares in a grid with grooves between them, each lit on its top and
    left edges and in shadow on the others."""
    gx = np.mod(dx + PITCH / 2, PITCH)       # (a square's middle at the lid's middle)
    gy = np.mod(dy + PITCH / 2, PITCH)
    colour = np.broadcast_to(TILE, dx.shape + (3,)).copy()
    lit = (gx < GROOVE_WIDTH + BEVEL) | (gy < GROOVE_WIDTH + BEVEL)
    shaded = (gx > PITCH - BEVEL) | (gy > PITCH - BEVEL)
    colour = np.where(shaded[..., None], colour * 0.72, colour)
    colour = np.where(lit[..., None], np.minimum(colour * 1.32, 255), colour)
    groove = (gx < GROOVE_WIDTH) | (gy < GROOVE_WIDTH)
    return np.where(groove[..., None], GROOVE, colour)


def icon():
    """The icon: the plate, the light around the lid, the lid (RGBA, SIZE square)."""
    size = SIZE * SCALE
    pixel = 1.0 / SCALE
    y, x = (np.mgrid[0:size, 0:size].astype(np.float32) + 0.5) / SCALE    # (in the 1024 icon's pixels)
    dx, dy = x - LID_X, y - LID_Y
    r = np.hypot(dx, dy)

    # the plate, from the night at the top to the green at the bottom, lit around the lid
    shade = np.clip((y - PLATE_MARGIN) / (SIZE - 2 * PLATE_MARGIN), 0, 1)[..., None]
    colour = PLATE_TOP * (1 - shade) + PLATE_BOTTOM * shade
    glow = np.exp(-np.maximum(r - LID_RADIUS, 0) / (GLOW_REACH / np.log(3)))
    colour = np.minimum(colour + GLOW * (0.85 * glow)[..., None], 255)

    # the rim: a rounded ring, lit on the top left of its outer half and the bottom right of its inner one
    across = np.clip((r - RIM_INNER) / (LID_RADIUS - RIM_INNER), 0, 1)
    facing = (dx * LIGHT[0] + dy * LIGHT[1]) / np.maximum(r, 1)
    rim = np.clip(IRON * (1 + 0.45 * facing * np.cos(np.pi * (1 - across)))[..., None], 0, 255)
    face = lid_face(dx, dy)
    across_hub = np.clip((dx * LIGHT[0] + dy * LIGHT[1]) / HUB_RADIUS, -1, 1)    # (lit from the top left)
    hub = np.clip(IRON * (0.94 + 0.14 * across_hub)[..., None], 0, 255)
    face = blend(face, hub, coverage(r - HUB_RADIUS, pixel))
    face = blend(face, GROOVE, coverage(np.abs(r - HUB_RADIUS) - 5, pixel))
    lid = blend(rim, face, coverage(r - RIM_INNER, pixel))
    lid = blend(lid, GROOVE, coverage(np.abs(r - RIM_INNER) - 5, pixel))
    colour = blend(colour, lid, coverage(r - LID_RADIUS, pixel))

    cover = coverage(rounded_square(x, y, PLATE_MARGIN, PLATE_MARGIN, SIZE - PLATE_MARGIN, SIZE - PLATE_MARGIN,
                                    PLATE_RADIUS), pixel)
    image = np.dstack([colour, cover * 255])
    image = Image.fromarray(np.clip(image, 0, 255).round().astype(np.uint8), "RGBA")
    return image.resize((SIZE, SIZE), Image.LANCZOS)


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    picture = icon()
    picture.save(OUT / f"{NAME}.png")
    picture.save(OUT / f"{NAME}.ico", sizes=[(side, side) for side in ICO_SIZES])
    picture.save(OUT / f"{NAME}.icns")
    print(f"written: {NAME}.png, {NAME}.ico, {NAME}.icns in {OUT.relative_to(ROOT)}")


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
