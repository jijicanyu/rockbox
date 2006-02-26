/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Dan Everton
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "lcd-sdl.h"
#include "uisdl.h"

int display_zoom = 1;

void sdl_update_rect(SDL_Surface *surface, int x_start, int y_start, int width,
                     int height, int max_x, int max_y,
                     unsigned long (*getpixel)(int, int))
{
    int x, y;
    int xmax, ymax;
    SDL_Rect dest;

    ymax = y_start + height;
    xmax = x_start + width;

    if(xmax > max_x)
        xmax = max_x;
    if(ymax >= max_y)
        ymax = max_y;

    SDL_LockSurface(surface);

    dest.w = display_zoom;
    dest.h = display_zoom;
    
    for (x = x_start; x < xmax; x++) {
        dest.x = x * display_zoom;

        for (y = y_start; y < ymax; y++) {
            dest.y = y * display_zoom;
            
            SDL_FillRect(surface, &dest, (Uint32)getpixel(x, y));
        }
    }

    SDL_UnlockSurface(surface);
}

void sdl_gui_update(SDL_Surface *surface, int x_start, int y_start, int width,
                    int height, int max_x, int max_y, int ui_x, int ui_y)
{
    int xmax, ymax;

    ymax = y_start + height;
    xmax = x_start + width;

    if(xmax > max_x)
        xmax = max_x;
    if(ymax >= max_y)
        ymax = max_y;

    SDL_Rect src = {x_start * display_zoom, y_start * display_zoom,
                    xmax * display_zoom, ymax * display_zoom};
    SDL_Rect dest= {(ui_x + x_start) * display_zoom, (ui_y + y_start) * display_zoom,
                    xmax * display_zoom, ymax * display_zoom};

    SDL_BlitSurface(surface, &src, gui_surface, &dest);
    SDL_UpdateRect(gui_surface, dest.x, dest.y, dest.w, dest.h);
    SDL_Flip(gui_surface);
}

/* set a range of bitmap indices to a gradient from startcolour to endcolour */
void sdl_set_gradient(SDL_Surface *surface, SDL_Color *start, SDL_Color *end,
                      int first, int steps)
{
    int i;
    SDL_Color palette[steps];

    for (i = 0; i < steps; i++) {
        palette[i].r = start->r + (end->r - start->r) * i / (steps - 1);
        palette[i].g = start->g + (end->g - start->g) * i / (steps - 1);
        palette[i].b = start->b + (end->b - start->b) * i / (steps - 1);
    }

    SDL_SetPalette(surface, SDL_LOGPAL|SDL_PHYSPAL, palette, first, steps);
}

