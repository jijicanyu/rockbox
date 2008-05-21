/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (c) 2008 by Michael Sevakis
 *
 * Gigabeat S MC13783 event descriptions
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "config.h"
#include "system.h"
#include "mc13783.h"
#include "mc13783-target.h"
#include "adc-target.h"
#include "button-target.h"
#include "usb-target.h"
#include "power-imx31.h"

/* Gigabeat S definitions for static MC13783 event registration */

static const struct mc13783_event mc13783_events[] =
{
    [MC13783_ADCDONE_EVENT] = /* ADC conversion complete */
    {
        .set  = MC13783_EVENT_SET0,
        .mask = MC13783_ADCDONEM,
        .callback = adc_done,
    },
    [MC13783_ONOFD1_EVENT] = /* Power button */
    {
        .set  = MC13783_EVENT_SET1,
        .mask = MC13783_ONOFD1M,
        .callback = button_power_event,
    },
#ifdef HAVE_HEADPHONE_DETECTION
    [MC13783_ONOFD2_EVENT] = /* Headphone jack */
    {
        .set  = MC13783_EVENT_SET1,
        .mask = MC13783_ONOFD2M,
        .callback = headphone_detect_event,
    },
#endif
    [MC13783_CHGDET_EVENT] = /* Charger detection */
    {
        .set  = MC13783_EVENT_SET0,
        .mask = MC13783_CHGDETM,
        .callback = charger_detect_event,
    },
    [MC13783_USB4V4_EVENT] = /* USB insertion */
    {
        .set  = MC13783_EVENT_SET0,
        .mask = MC13783_USB4V4M,
        .callback = usb_connect_event,
    },
};

const struct mc13783_event_list mc13783_event_list =
{
    .count = ARRAYLEN(mc13783_events),
    .events = mc13783_events
};