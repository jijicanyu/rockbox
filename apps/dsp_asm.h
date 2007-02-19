/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Thom Johansen
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include <config.h>

#ifndef _DSP_ASM_H
#define _DSP_ASM_H

#if defined(CPU_COLDFIRE) || defined(CPU_ARM)
#define DSP_HAVE_ASM_CROSSFEED
void apply_crossfeed(int32_t* src[], int count);
#endif

#if defined (CPU_COLDFIRE)
#define DSP_HAVE_ASM_RESAMPLING
int dsp_downsample(int channels, int count, void *resample_data,
                   int32_t **src, int32_t **dst);
int dsp_upsample(int channels, int count, void *resample_data,
                 int32_t **src, int32_t **dst);
#endif

#endif /* _DSP_ASM_H */

