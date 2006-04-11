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

#include <inttypes.h>
#include "config.h"
#include "eq.h"

/* Coef calculation taken from Audio-EQ-Cookbook.txt by Robert Bristow-Johnson.
   Slightly faster calculation can be done by deriving forms which use tan()
   instead of cos() and sin(), but the latter are far easier to use when doing
   fixed point math, and performance is not a big point in the calculation part.
   All the 'a' filter coefficients are negated so we can use only additions
   in the filtering equation.
   We realise the filters as a second order direct form 1 structure. Direct
   form 1 was chosen because of better numerical properties for fixed point
   implementations.
 */

#define DIV64(x, y, z) (long)(((long long)(x) << (z))/(y))
/* This macro requires the EMAC unit to be in fractional mode
   when the coef generator routines are called. If this can't be guaranteeed,
   then add "&& 0" below. This will use a slower coef calculation on Coldfire.
 */
#if defined(CPU_COLDFIRE) && !defined(SIMULATOR)
#define FRACMUL(x, y) \
({ \
    long t; \
    asm volatile ("mac.l    %[a], %[b], %%acc0\n\t" \
                  "movclr.l %%acc0, %[t]\n\t" \
                  : [t] "=r" (t) : [a] "r" (x), [b] "r" (y)); \
    t; \
})
#else
#define FRACMUL(x, y) ((long)(((((long long) (x)) * ((long long) (y))) >> 31)))
#endif

/* TODO: replaygain.c has some fixed point routines. perhaps we could reuse
   them? */

/* Inverse gain of circular cordic rotation in s0.31 format. */
static const long cordic_circular_gain = 0xb2458939; /* 0.607252929 */

/* Table of values of atan(2^-i) in 0.32 format fractions of pi where pi = 0xffffffff / 2 */
static const unsigned long atan_table[] = {
    0x1fffffff, /* +0.785398163 (or pi/4) */
    0x12e4051d, /* +0.463647609 */
    0x09fb385b, /* +0.244978663 */
    0x051111d4, /* +0.124354995 */
    0x028b0d43, /* +0.062418810 */
    0x0145d7e1, /* +0.031239833 */
    0x00a2f61e, /* +0.015623729 */
    0x00517c55, /* +0.007812341 */
    0x0028be53, /* +0.003906230 */
    0x00145f2e, /* +0.001953123 */
    0x000a2f98, /* +0.000976562 */
    0x000517cc, /* +0.000488281 */
    0x00028be6, /* +0.000244141 */
    0x000145f3, /* +0.000122070 */
    0x0000a2f9, /* +0.000061035 */
    0x0000517c, /* +0.000030518 */
    0x000028be, /* +0.000015259 */
    0x0000145f, /* +0.000007629 */
    0x00000a2f, /* +0.000003815 */
    0x00000517, /* +0.000001907 */
    0x0000028b, /* +0.000000954 */
    0x00000145, /* +0.000000477 */
    0x000000a2, /* +0.000000238 */
    0x00000051, /* +0.000000119 */
    0x00000028, /* +0.000000060 */
    0x00000014, /* +0.000000030 */
    0x0000000a, /* +0.000000015 */
    0x00000005, /* +0.000000007 */
    0x00000002, /* +0.000000004 */
    0x00000001, /* +0.000000002 */
    0x00000000, /* +0.000000001 */
    0x00000000, /* +0.000000000 */
};

/**
 * Implements sin and cos using CORDIC rotation.
 *
 * @param phase has range from 0 to 0xffffffff, representing 0 and
 *        2*pi respectively.
 * @param cos return address for cos
 * @return sin of phase, value is a signed value from LONG_MIN to LONG_MAX,
 *         representing -1 and 1 respectively. 
 */
long fsincos(unsigned long phase, long *cos) {
    int32_t x, x1, y, y1;
    unsigned long z, z1;
    int i;

    /* Setup initial vector */
    x = cordic_circular_gain;
    y = 0;
    z = phase;

    /* The phase has to be somewhere between 0..pi for this to work right */
    if (z < 0xffffffff / 4) {
        /* z in first quadrant, z += pi/2 to correct */
        x = -x;
        z += 0xffffffff / 4;
    } else if (z < 3 * (0xffffffff / 4)) {
        /* z in third quadrant, z -= pi/2 to correct */
        z -= 0xffffffff / 4;
    } else {
        /* z in fourth quadrant, z -= 3pi/2 to correct */
        x = -x;
        z -= 3 * (0xffffffff / 4);
    }

    /* Each iteration adds roughly 1-bit of extra precision */
    for (i = 0; i < 31; i++) {
        x1 = x >> i;
        y1 = y >> i;
        z1 = atan_table[i];

        /* Decided which direction to rotate vector. Pivot point is pi/2 */
        if (z >= 0xffffffff / 4) {
            x -= y1;
            y += x1;
            z -= z1;
        } else {
            x += y1;
            y -= x1;
            z += z1;
        }
    }

    *cos = x;

    return y;
}

/* Fixed point square root via Newton-Raphson.
 * Output is in same fixed point format as input. 
 * fracbits specifies number of fractional bits in argument.
 */
static long fsqrt(long a, unsigned int fracbits)
{
    long b = a/2 + (1 << fracbits); /* initial approximation */
    unsigned n;
    const unsigned iterations = 4;
    
    for (n = 0; n < iterations; ++n)
        b = (b + DIV64(a, b, fracbits))/2;

    return b;
}

short dbtoatab[49] = {
    2058, 2180, 2309, 2446, 2591, 2744, 2907, 3079, 3261, 3455, 3659, 3876,
    4106, 4349, 4607, 4880, 5169, 5475, 5799, 6143, 6507, 6893, 7301, 7734,
    8192, 8677, 9192, 9736, 10313, 10924, 11572, 12257, 12983, 13753, 14568,
    15431, 16345, 17314, 18340, 19426, 20577, 21797, 23088, 24456, 25905, 27440,
    29066, 30789, 32613
};

/* Function for converting dB to squared amplitude factor (A = 10^(dB/40)).
   Range is -24 to 24 dB. If gain values outside of this is needed, the above
   table needs to be extended.
   Parameter format is s15.16 fixed point. Return format is s2.29 fixed point.
 */
static long dbtoA(long db)
{
    const unsigned long bias = 24 << 16;
    unsigned short frac = (db + bias) & 0x0000ffff;
    unsigned short pos = (db + bias) >> 16;
    short diff = dbtoatab[pos + 1] - dbtoatab[pos];
    
    return (dbtoatab[pos] << 16) + frac*diff;
}

/* Calculate first order shelving filter coefficients.
   cutoff is a value from 0 to 0x80000000, where 0 represents 0 hz and
   0x80000000 represents nyquist (samplerate/2).
   ad is gain at 0 hz, and an is gain at Nyquist frequency. Both are s3.27
   format. 
   c is a pointer where the coefs will be stored. The coefs are s0.31 format.
   Note that the filter is not compatible with the eq_filter routine.
 */
void filter_bishelf_coefs(unsigned long cutoff, long ad, long an, int32_t *c)
{
    const long one = 1 << 27;
    long a0, a1;
    long b0, b1;
    long s, cs;
    s = fsincos(cutoff, &cs) >> 4;
    cs = one + (cs >> 4);

    /* For max A = 4 (24 dB) */
    b0 = (FRACMUL(an, cs) << 4) + (FRACMUL(ad, s) << 4);
    b1 = (FRACMUL(ad, s) << 4) - (FRACMUL(an, cs) << 4);
    a0 = s + cs;
    a1 = s - cs;

    c[0] = DIV64(b0, a0, 31);
    c[1] = DIV64(b1, a0, 31);
    c[2] = -DIV64(a1, a0, 31);
}

/* Calculate second order section peaking filter coefficients.
   cutoff is a value from 0 to 0x80000000, where 0 represents 0 hz and
   0x80000000 represents nyquist (samplerate/2).
   Q is an unsigned 16.16 fixed point number, lower bound is artificially set
   at 0.5.
   db is s15.16 fixed point and describes gain/attenuation at peak freq.
   c is a pointer where the coefs will be stored.
 */
void eq_pk_coefs(unsigned long cutoff, unsigned long Q, long db, int32_t *c)
{
    long cc;
    const long one = 1 << 28; /* s3.28 */
    const long A = dbtoA(db);
    const long alpha = DIV64(fsincos(cutoff, &cc), 2*Q, 15); /* s1.30 */
    int32_t a0, a1, a2; /* these are all s3.28 format */
    int32_t b0, b1, b2;

    /* possible numerical ranges listed after each coef */
    b0 = one + FRACMUL(alpha, A);     /* [1.25..5] */
    b1 = a1 = -2*(cc >> 3);           /* [-2..2] */
    b2 = one - FRACMUL(alpha, A);     /* [-3..0.75] */
    a0 = one + DIV64(alpha, A, 27);   /* [1.25..5] */
    a2 = one - DIV64(alpha, A, 27);   /* [-3..0.75] */

    c[0] = DIV64(b0, a0, 28);
    c[1] = DIV64(b1, a0, 28);
    c[2] = DIV64(b2, a0, 28);
    c[3] = DIV64(-a1, a0, 28);
    c[4] = DIV64(-a2, a0, 28);
}

/* Calculate coefficients for lowshelf filter */
void eq_ls_coefs(unsigned long cutoff, unsigned long Q, long db, int32_t *c)
{
    long cs;
    const long one = 1 << 24; /* s7.24 */
    const long A = dbtoA(db);
    const long alpha = DIV64(fsincos(cutoff, &cs), 2*Q, 15); /* s1.30 */
    const long ap1 = (A >> 5) + one;
    const long am1 = (A >> 5) - one;
    const long twosqrtalpha = 2*(FRACMUL(fsqrt(A >> 5, 24), alpha) << 1);
    int32_t a0, a1, a2; /* these are all s7.24 format */
    int32_t b0, b1, b2;

    b0 = FRACMUL(A, ap1 - FRACMUL(am1, cs) + twosqrtalpha) << 2;
    b1 = FRACMUL(A, am1 - FRACMUL(ap1, cs)) << 3; 
    b2 = FRACMUL(A, ap1 - FRACMUL(am1, cs) - twosqrtalpha) << 2; 
    a0 = ap1 + FRACMUL(am1, cs) + twosqrtalpha; 
    a1 = -2*((am1 + FRACMUL(ap1, cs))); 
    a2 = ap1 + FRACMUL(am1, cs) - twosqrtalpha;

    c[0] = DIV64(b0, a0, 24);
    c[1] = DIV64(b1, a0, 24);
    c[2] = DIV64(b2, a0, 24);
    c[3] = DIV64(-a1, a0, 24);
    c[4] = DIV64(-a2, a0, 24);
}

/* Calculate coefficients for highshelf filter */
void eq_hs_coefs(unsigned long cutoff, unsigned long Q, long db, int32_t *c)
{
    long cs;
    const long one = 1 << 24; /* s7.24 */
    const long A = dbtoA(db);
    const long alpha = DIV64(fsincos(cutoff, &cs), 2*Q, 15); /* s1.30 */
    const long ap1 = (A >> 5) + one;
    const long am1 = (A >> 5) - one;
    const long twosqrtalpha = 2*(FRACMUL(fsqrt(A >> 5, 24), alpha) << 1);
    int32_t a0, a1, a2; /* these are all s7.24 format */
    int32_t b0, b1, b2;

    b0 = FRACMUL(A, ap1 + FRACMUL(am1, cs) + twosqrtalpha) << 2;
    b1 = -FRACMUL(A, am1 + FRACMUL(ap1, cs)) << 3;
    b2 = FRACMUL(A, ap1 + FRACMUL(am1, cs) - twosqrtalpha) << 2;
    a0 = ap1 - FRACMUL(am1, cs) + twosqrtalpha;
    a1 = 2*((am1 - FRACMUL(ap1, cs)));
    a2 = ap1 - FRACMUL(am1, cs) - twosqrtalpha;

    c[0] = DIV64(b0, a0, 24);
    c[1] = DIV64(b1, a0, 24);
    c[2] = DIV64(b2, a0, 24);
    c[3] = DIV64(-a1, a0, 24);
    c[4] = DIV64(-a2, a0, 24);
}

#if (!defined(CPU_COLDFIRE) && !defined(CPU_ARM)) || defined(SIMULATOR)
void eq_filter(int32_t **x, struct eqfilter *f, unsigned num,
               unsigned channels, unsigned shift)
{
    unsigned c, i;
    long long acc;

    /* Direct form 1 filtering code.
       y[n] = b0*x[i] + b1*x[i - 1] + b2*x[i - 2] + a1*y[i - 1] + a2*y[i - 2],
       where y[] is output and x[] is input.
     */

    for (c = 0; c < channels; c++) {
        for (i = 0; i < num; i++) {
            acc  = (long long) x[c][i] * f->coefs[0];
            acc += (long long) f->history[c][0] * f->coefs[1];
            acc += (long long) f->history[c][1] * f->coefs[2];
            acc += (long long) f->history[c][2] * f->coefs[3];
            acc += (long long) f->history[c][3] * f->coefs[4];
            f->history[c][1] = f->history[c][0];
            f->history[c][0] = x[c][i];
            f->history[c][3] = f->history[c][2];
            x[c][i] = (acc << shift) >> 32;
            f->history[c][2] = x[c][i];
        }
    }
}
#endif

