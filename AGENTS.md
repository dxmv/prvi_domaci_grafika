# RAFGL Overview for Agents

This document explains how the RAFGL mini-engine is wired so you can reason about existing projects or add new behaviour without re-reading the entire source tree.

## High-level Flow
1. `main.c` instantiates a `rafgl_game_t`, initializes GLFW/OpenGL through `rafgl_game_init`, registers at least one state with `rafgl_game_add_named_game_state`, and hands control to `rafgl_game_start`. See `main.c` for the minimal 1000×1000 demo setup.
2. `rafgl_game_start` (defined in `include/rafgl.h`) enters the main loop: it polls input, tracks FPS, updates the active state, renders it, swaps buffers, and processes requested state transitions.
3. Game states are plain C function bundles (`init`, `update`, `render`, `cleanup`) defined in `rafgl_game_state_t`. They receive a GLFW window pointer plus an opaque `args` pointer so you can pass custom context data between states.

## Life of a Frame
1. **Input snapshot** – `rafgl_game_data_t` is filled with framebuffer size, mouse position/buttons, and keyboard arrays (`keys_down` for the current state, `keys_pressed` for rising edges). RAFGL services the GLFW callbacks so user code only reads the struct.
2. **State update** – RAFGL calls `state->update(window, delta_time, &game_data, args)`. Update functions can request a state change via `rafgl_game_request_state_change`.
3. **CPU raster work** – Typical 2D projects keep a `rafgl_raster_t` (CPU pixel array). You address pixels with the `pixel_at_m`/`pixel_at_pm` macros, as shown in `src/main_state.c` where every pixel is painted red in `main_state_update`.
4. **GPU upload and blit** – In `render`, call `rafgl_texture_load_from_raster` to upload the raster to a `rafgl_texture_t`, then display it with `rafgl_texture_show`. RAFGL hides VAO/VBO/Shader details by using an internal fullscreen quad and a stock shader.
5. **Swap** – RAFGL swaps the GLFW buffers and loops until the window closes.

## Core Data Structures
- `rafgl_raster_t`: owns CPU-side pixel memory (RGBA8). Allocate with `rafgl_raster_init`, free via `rafgl_raster_cleanup`, duplicate with `rafgl_raster_copy`, or fill from disk images using `rafgl_raster_load_from_image`.
- `rafgl_texture_t`: wraps a GL texture id. Initialize with `rafgl_texture_init`, feed it from a raster using `rafgl_texture_load_from_raster`, display via `rafgl_texture_show`, and release with `rafgl_texture_cleanup`.
- `rafgl_game_state_t`: holds function pointers for lifecycle callbacks plus an `args` pointer and id. The macro `rafgl_game_add_named_game_state(game, main_state)` expands to the proper registration call.
- `rafgl_game_data_t`: per-frame transient data (framebuffer size, cursor position, mouse button flags, `keys_down`, `keys_pressed`).
- `rafgl_list_t`: minimal intrusive list implementation for storing states, mesh data, OBJ indices, etc.

## Drawing Helpers
RAFGL exposes a lightweight software-rendering toolbox that operates directly on rasters:
- 2D primitives: `rafgl_raster_draw_line`, `_draw_circle`, `_draw_rectangle`, `rafgl_raster_box_blur`, bilinear upsampling, color lerp/saturate helpers, and interpolation-friendly macros.
- Sprites/text: load sprite sheets with `rafgl_spritesheet_init` and blit frames using `rafgl_raster_draw_spritesheet`. Text is drawn by sampling mono-space font atlases (RAFGL auto-loads three sizes from `res/fonts/chars-*.png`).
- Buttons: `rafgl_button_t` with `rafgl_button_innit`, `_show`, and `_check` for simple UI interactions backed by the shared input snapshot.
- Sampling: `rafgl_point_sample` and `rafgl_bilinear_sample` map normalized UVs to pixels, useful for effects like the provided separable box blur implementation.

## 3D Support
While the sample game only uses the raster/texture path, RAFGL also exposes OpenGL-ready helpers:
- `rafgl_meshPUN_t` plus loaders for planes, cubes, OBJ files, and terrains derived from heightmaps. They emit VAOs with P (position) / U (texture coord) / N (normal) attributes so you can plug them into your own shaders.
- `rafgl_texture_load_cubemap(_named)` for skyboxes and reflective materials.
- Simple framebuffer wrappers (`rafgl_framebuffer_simple_create`, `_multitarget_create`) for off-screen rendering.

## Logging & Timing
RAFGL opens log files per severity (`RAFGL_ERROR`, `RAFGL_WARNING`, `RAFGL_INFO`). Use `rafgl_log(level, fmt, ...)` for consistent diagnostics; enabling `rafgl_log_fps(1)` prints periodic FPS stats. Timing is driven by GLFW’s `glfwGetTime` and `delta_time` passed into every update call.

## Directory Layout Highlights
- `main.c`: entry point wiring RAFGL into the current project.
- `src/main_state.c`: reference implementation of the state lifecycle for raster-based drawing.
- `include/rafgl.h`: single-header implementation of the engine (data types, helpers, and the main loop).
- `include/stb_image*.h`: vendor libraries for image IO.
- `res/`: expected location for font atlases, textures, cubemaps, OBJ files, etc. (not all assets are committed).

## Typical Workflow for a New Feature
1. Create a new state header/source pair following `main_state`. Implement the four callbacks and keep any persistent state static (or hang it off the `args` pointer).
2. Allocate rasters/textures you need inside `*_init`, mutate them in `*_update`, and draw/upload in `*_render`.
3. For interactive features, use `game_data` to inspect input. Track edges with `keys_pressed` and hold states via `keys_down`/mouse booleans.
4. Call `rafgl_game_add_named_game_state` in `main.c` for each new state. Jump between them from inside an update via `rafgl_game_request_state_change(target_index, args)`.
5. Use the drawing helpers, sprite sheets, OBJ loader, or mesh builders as needed to avoid re-writing boilerplate.

Understanding this control flow should give you everything required to reason about RAFGL projects or automate edits—they all plug into the same C API surface described above.
