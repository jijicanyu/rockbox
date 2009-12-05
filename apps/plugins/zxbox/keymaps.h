#ifndef KEYMAPS_H
#define KEYMAPS_H

#if (CONFIG_KEYPAD == IPOD_4G_PAD) || (CONFIG_KEYPAD == IPOD_3G_PAD) || \
    (CONFIG_KEYPAD == IPOD_1G2G_PAD)

#define ZX_UP        BUTTON_MENU
#define ZX_DOWN        BUTTON_PLAY
#define ZX_SELECT    BUTTON_SELECT
#define ZX_LEFT        BUTTON_LEFT
#define ZX_RIGHT    BUTTON_RIGHT

#elif CONFIG_KEYPAD == IRIVER_H100_PAD || CONFIG_KEYPAD == IRIVER_H300_PAD

#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT  BUTTON_ON
#define ZX_MENU    BUTTON_MODE

#elif CONFIG_KEYPAD == GIGABEAT_PAD

#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT     BUTTON_SELECT
#define ZX_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == GIGABEAT_S_PAD

#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT     BUTTON_SELECT
#define ZX_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD

#define ZX_RIGHT      BUTTON_RIGHT
#define ZX_LEFT    BUTTON_LEFT
#define ZX_UP    BUTTON_UP
#define ZX_DOWN   BUTTON_DOWN
#define ZX_SELECT     BUTTON_SELECT
#define ZX_MENU    BUTTON_PLAY

#elif CONFIG_KEYPAD == RECORDER_PAD
#define ZX_SELECT BUTTON_PLAY
#define ZX_MENU BUTTON_F1
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif CONFIG_KEYPAD == ARCHOS_AV300_PAD
#define ZX_SELECT BUTTON_SELECT
#define ZX_MENU BUTTON_OFF
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif CONFIG_KEYPAD == ONDIO_PAD
#define ZX_SELECT BUTTON_MENU
#define ZX_MENU BUTTON_OFF
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif CONFIG_KEYPAD == IRIVER_IFP7XX_PAD
#define ZX_SELECT BUTTON_SELECT 
#define ZX_MENU BUTTON_PLAY
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif CONFIG_KEYPAD == IRIVER_H10_PAD
#define ZX_SELECT BUTTON_REW
#define ZX_MENU BUTTON_FF
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_SCROLL_UP
#define ZX_DOWN BUTTON_SCROLL_DOWN

#elif (CONFIG_KEYPAD == SANSA_E200_PAD) || \
      (CONFIG_KEYPAD == SANSA_C200_PAD) || \
      (CONFIG_KEYPAD == SANSA_CLIP_PAD) || \
      (CONFIG_KEYPAD == SANSA_M200_PAD)
#define ZX_SELECT BUTTON_SELECT
#define ZX_MENU BUTTON_POWER
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)
#define ZX_SELECT BUTTON_SELECT
#define ZX_MENU (BUTTON_HOME|BUTTON_REPEAT)
#define ZX_LEFT BUTTON_LEFT
#define ZX_RIGHT BUTTON_RIGHT
#define ZX_UP BUTTON_UP
#define ZX_DOWN BUTTON_DOWN

#elif CONFIG_KEYPAD == MROBE500_PAD
#define ZX_UP           BUTTON_RC_PLAY
#define ZX_DOWN         BUTTON_RC_DOWN
#define ZX_LEFT         BUTTON_RC_REW
#define ZX_RIGHT        BUTTON_RC_FF
#define ZX_SELECT    BUTTON_RC_MODE
#define ZX_MENU         (BUTTON_POWER | BUTTON_REL)

#elif CONFIG_KEYPAD == MROBE100_PAD
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_SELECT
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == IAUDIO_M3_PAD
#define ZX_UP           BUTTON_RC_VOL_UP
#define ZX_DOWN         BUTTON_RC_VOL_DOWN
#define ZX_LEFT         BUTTON_RC_REW
#define ZX_RIGHT        BUTTON_RC_FF
#define ZX_SELECT       BUTTON_RC_PLAY
#define ZX_MENU         BUTTON_RC_REC

#elif CONFIG_KEYPAD == COWOND2_PAD
#define ZX_MENU         (BUTTON_MENU|BUTTON_REL)

#elif CONFIG_KEYPAD == IAUDIO67_PAD
#define ZX_UP           BUTTON_STOP
#define ZX_DOWN         BUTTON_PLAY
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_VOLUP
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define ZX_UP      BUTTON_UP
#define ZX_DOWN    BUTTON_DOWN
#define ZX_LEFT    BUTTON_LEFT
#define ZX_RIGHT   BUTTON_RIGHT
#define ZX_SELECT     BUTTON_SELECT
#define ZX_MENU    BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_HDD1630_PAD

#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_SELECT
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == PHILIPS_SA9200_PAD
#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_PREV
#define ZX_RIGHT        BUTTON_NEXT
#define ZX_SELECT       BUTTON_RIGHT
#define ZX_MENU         BUTTON_MENU

#elif CONFIG_KEYPAD == ONDAVX747_PAD
#define ZX_MENU         (BUTTON_MENU|BUTTON_REL)

#elif CONFIG_KEYPAD == ONDAVX777_PAD
#define ZX_MENU         BUTTON_POWER

#elif CONFIG_KEYPAD == SAMSUNG_YH_PAD

#define ZX_UP           BUTTON_UP
#define ZX_DOWN         BUTTON_DOWN
#define ZX_LEFT         BUTTON_LEFT
#define ZX_RIGHT        BUTTON_RIGHT
#define ZX_SELECT       BUTTON_PLAY
#define ZX_MENU         BUTTON_FFWD

#else
#error Keymap not defined!

#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef ZX_UP
#define ZX_UP           BUTTON_TOPMIDDLE
#endif
#ifndef ZX_DOWN
#define ZX_DOWN         BUTTON_BOTTOMMIDDLE
#endif
#ifndef ZX_LEFT
#define ZX_LEFT         BUTTON_MIDLEFT
#endif
#ifndef ZX_RIGHT
#define ZX_RIGHT        BUTTON_MIDRIGHT
#endif
#ifndef ZX_SELECT
#define ZX_SELECT       BUTTON_CENTER
#endif
#ifndef ZX_MENU
#define ZX_MENU         (BUTTON_TOPLEFT|BUTTON_REL)
#endif
#endif

#endif
