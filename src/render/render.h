/**
 * render.h
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * The rendering module, primarily for rendering a single frame and drawing it
 * to the screen. Based on information in the game model. Currently doesn't
 * minimize rendering
 */

#include "../model/model.h"
#include "../raster/raster.h"

/**
 * Get the buffer currently being rendered to.
 * Call this in render() to get the target buffer.
 */
UINT8 *get_render_buffer(void);

/**
 * Get the buffer currently displayed on screen.
 * Call this in VBL handler to get the buffer to flip to.
 */
UINT8 *get_display_buffer(void);

/**
 * Mark rendering as complete. Call after render() finishes.
 * The actual buffer swap should happen in VBL interrupt.
 */
void mark_render_complete(void);

/**
 * Swap buffers - call this in VBL interrupt handler.
 * Swaps render_index and display_index, then updates video base.
 */
void swap_buffers(void);

/**
 * The master rendering function, which will render a single frame. Delegates
 * to the remaining render functions in render, only renders objects within
 * the camera's active indices as dictated by the model. Currently clears
 * the screen first before we implement render minimization
 *
 * Params:
 *     Model *model:
 *         The model snapshot to render
 *     UINT8 base:
 *         The base address of the framebuffer to render to
 */
void render(const Model *model, UINT8 *base);

/**
 * Render Geo to the framebuffer
 *
 * Params:
 *     Geo *geo:
 *         The Geo (Player) to render
 *     Camera *camera:
 *         The current camera state for coordinate translation
 */
void render_geo(const Geo *geo, const Camera *camera, UINT8 *base);

/**
 * Render a block to the framebuffer
 *
 * Params:
 *     Block *block:
 *         The block to render
 *     Camera *camera:
 *         The current camera state for coordinate translation
 */
void render_block(const Block *block, const Camera *camera, UINT8 *base);

/**
 * Render a spike to the framebuffer
 *
 * Params:
 *     Spike *spike:
 *         The spike to render
 *     Camera *camera:
 *         The current camera state for coordinate translation
 */
void render_spike(const Spike *spike, const Camera *camera, UINT8 *base);

/**
 * Render a lava to the framebuffer
 *
 * Params:
 *     Lava *lava:
 *         The lava to render
 *     Camera *camera:
 *         The current camera state for coordinate translation
 */
void render_lava(const Lava *lava, const Camera *camera, UINT8 *base);

/**
 * Render the ground to the framebuffer
 *
 * Params:
 *     Model *model:
 *         The model snapshot to render
 *     Camera *camera:
 *         The current camera state for coordinate translation
 */
void render_ground(const Model *model, UINT8 *base);
