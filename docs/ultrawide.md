# Ultrawide (experimental, game 1.4.3)

In **F1 → Screen → Picture**, set **Aspect ratio** to **Window (ultrawide)**, then choose a matching window size or
use F11 for borderless fullscreen. The camera follows the window when it is wider than 16:9, retaining its vertical
field of view and showing more of the world at the sides. The game's original 16:9 mode remains the default.

**Game resolution** controls pixel count separately. For example, use 3440×1440 with a 3440×1440 window for a native
ultrawide canvas, or keep 1920×1080 to render fewer pixels and scale the wider camera view to that window. Resolution
changes require a restart; aspect changes and window resizing apply live. 2560×1080, 3440×1440, 3840×1600,
5120×1440 and 5120×2160 are offered in the menu.

The file setting next to `totk.exe` is:

```ini
[display]
aspect = window
window_size = 3440x1440
output_res = 3440x1440
```

`TOTK_ASPECT=window` overrides the file. `aspect = 16:9` restores the original behaviour. Windows at 16:9 or narrower
keep the original camera; this is a horizontal ultrawide option, not a taller-aspect or Vert+ implementation.

## Limitations

- Only game 1.4.3 has checked aspect patch sites. On 1.2.1 the row is disabled and the original camera is used.
- Known HUD shaders are narrowed to a centred 16:9 region on the final canvas. Some world-space markers,
  pixel-snapped effects, map elements and full-screen UI transitions may still be misplaced or fail to cover the sides.
- Pre-rendered movies, every cutscene and every menu need further testing. Do not assume all cutscene framing is corrected.
- The game's internal render scale, dynamic resolution and 4096-pixel scene limit still apply.

## Implementation and checks

`runtime/aspect.c` publishes the current window ratio on the patch page. Four checked replacements in
`recomp/patches-v143.txt` change the camera setup/update and viewport aspect producers; they preserve their scratch
register and leave the original instruction selected at 16:9. `runtime/vk_exec_draw.c` identifies the final canvas and
known UI vertex shaders, then transforms their viewport and scissor together. Scene and post-processing draws stay
unchanged. Sites and shader identifiers follow the MIT-licensed MacNativeGames implementation; attribution is in
`THIRD_PARTY_NOTICES.md`.

`python tests/aspect_check.py` checks setting changes, unsupported builds, 21:9/32:9 resizing and centred/clipped HUD
rectangles without game data. Set `CC` to an llvm-mingw compiler when MSYS2 Clang is not installed. Live checks should
compare the same scene at 16:9 and ultrawide and exercise gameplay, the map, inventory, a dialogue and resizing back.
