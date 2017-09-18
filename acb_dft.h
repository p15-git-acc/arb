/*
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef ACB_DFT_H
#define ACB_DFT_H

#ifdef ACB_DFT_INLINES_C
#define ACB_DFT_INLINE
#else
#define ACB_DFT_INLINE static __inline__
#endif

#include "acb.h"
#include "dlog.h"

#ifdef __cplusplus
extern "C" {
#endif

void _acb_dft_pol(acb_ptr w, acb_srcptr v, slong dv, acb_srcptr z, slong dz, slong len, slong prec);
void acb_dft_pol(acb_ptr w, acb_srcptr v, slong len, slong prec);
void acb_dft_crt(acb_ptr w, acb_srcptr v, slong len, slong prec);
void acb_dft_cyc(acb_ptr w, acb_srcptr v, slong len, slong prec);
void acb_dft_rad2(acb_ptr v, int e, slong prec);
void acb_dft_bluestein(acb_ptr w, acb_srcptr v, slong len, slong prec);
void acb_dft_prod(acb_ptr w, acb_srcptr v, slong * cyc, slong num, slong prec);

void acb_dft_convol_naive(acb_ptr w, acb_srcptr f, acb_srcptr g, slong len, slong prec);
void acb_dft_convol_rad2(acb_ptr w, acb_srcptr f, acb_srcptr g, slong len, slong prec);

#define CRT_MAX 15
typedef struct
{
    int num;
    nmod_t n;
    slong m[CRT_MAX];
    ulong M[CRT_MAX];
    ulong vM[CRT_MAX];
}
crt_struct;

typedef crt_struct crt_t[1];

void crt_init(crt_t c, ulong n);

void crt_decomp(acb_ptr y, acb_srcptr x, slong dx, const crt_t c, ulong len);
void crt_recomp(acb_ptr y, acb_srcptr x, const crt_t c, ulong len);

typedef struct acb_dft_step_struct acb_dft_step_struct;
typedef acb_dft_step_struct * acb_dft_step_ptr;

typedef struct
{
    slong n;
    int zclear;
    acb_ptr z;
    slong num;
    acb_dft_step_ptr cyc;
}
acb_dft_cyc_struct;

typedef acb_dft_cyc_struct acb_dft_cyc_t[1];

typedef struct
{
    int e;
    slong n; /* = 1 << e */
    slong nz; /* = n but could be bigger */
    acb_ptr z;
}
acb_dft_rad2_struct;

typedef acb_dft_rad2_struct acb_dft_rad2_t[1];

typedef struct
{
    slong n;
    acb_ptr z;
    acb_dft_rad2_t rad2;
}
acb_dft_bluestein_struct;

typedef acb_dft_bluestein_struct acb_dft_bluestein_t[1];

typedef struct
{
    slong n;
    slong num;
    acb_dft_step_ptr cyc;
}
acb_dft_prod_struct;

typedef acb_dft_prod_struct acb_dft_prod_t[1];

typedef struct
{
    slong n;
    crt_t c;
    slong dv;
    /* then a product */
    acb_dft_step_ptr cyc;
}
acb_dft_crt_struct;

typedef acb_dft_crt_struct acb_dft_crt_t[1];

typedef struct
{
    slong n;
    slong dv;
    int zclear;
    acb_ptr z;
    slong dz;
}
acb_dft_pol_struct;

typedef acb_dft_pol_struct acb_dft_pol_t[1];

typedef struct
{
    int type;
    union
    {
        acb_dft_rad2_t rad2;
        acb_dft_cyc_t cyc;
        acb_dft_prod_t prod;
        acb_dft_crt_t crt;
        acb_dft_pol_t pol;
    } t;
}
acb_dft_pre_struct;

typedef acb_dft_pre_struct acb_dft_pre_t[1];

/* covers both product and cyclic case */
struct
acb_dft_step_struct
{
    /* [G:H] */
    slong m;
    /* card H */
    slong M;
    slong dv; /* = M for prod, also = M if cyc is reordered */
    /* pointer on some roots of unity, if needed */
    acb_srcptr z;
    /* index of mM in z */
    slong dz;
    /* dft to call on H */
    acb_dft_pre_t pre;
    /* dft to call on G/H ? */
};

/*typedef acb_dft_pre_struct acb_dft_pre_t[1];*/

#define DFT_VERB 0

enum
{
    DFT_POL, DFT_CYC, DFT_PROD, DFT_CRT , DFT_RAD2 /*, DFT_CONV */
};

void acb_dft_step(acb_ptr w, acb_srcptr v, acb_dft_step_ptr cyc, slong num, slong prec);

void acb_dft_precomp(acb_ptr w, acb_srcptr v, const acb_dft_pre_t pre, slong prec);

ACB_DFT_INLINE void
acb_dft_pol_precomp(acb_ptr w, acb_srcptr v, const acb_dft_pol_t pol, slong prec)
{
    _acb_dft_pol(w, v, pol->dv, pol->z, pol->dz, pol->n, prec);
}
ACB_DFT_INLINE void
acb_dft_cyc_precomp(acb_ptr w, acb_srcptr v, const acb_dft_cyc_t cyc, slong prec)
{
    acb_dft_step(w, v, cyc->cyc, cyc->num, prec);
}
void acb_dft_rad2_precomp(acb_ptr v, const acb_dft_rad2_t rad2, slong prec);
void acb_dft_crt_precomp(acb_ptr w, acb_srcptr v, const acb_dft_crt_t crt, slong prec);
void acb_dft_prod_precomp(acb_ptr w, acb_srcptr v, const acb_dft_prod_t prod, slong prec);

void acb_dft_inverse_rad2_precomp(acb_ptr v, const acb_dft_rad2_t rad2, slong prec);
void acb_dft_convol_rad2_precomp(acb_ptr w, acb_srcptr f, acb_srcptr g, slong len, const acb_dft_rad2_t, slong prec);

void _acb_dft_precomp_init(acb_dft_pre_t pre, slong dv, acb_ptr z, slong dz, slong len, slong prec);
void acb_dft_precomp_init(acb_dft_pre_t pre, slong len, slong prec);
void acb_dft_precomp_clear(acb_dft_pre_t pre);

acb_dft_step_ptr _acb_dft_steps_prod(slong * m, slong num, slong prec);

ACB_DFT_INLINE void
acb_dft_prod_init(acb_dft_prod_t t, slong * cyc, slong num, slong prec)
{
    t->num = num;
    t->cyc = _acb_dft_steps_prod(cyc, num, prec);
}

void acb_dft_prod_clear(acb_dft_prod_t t);

void _acb_dft_cyc_init_z_fac(acb_dft_cyc_t t, n_factor_t fac, slong dv, acb_ptr z, slong dz, slong len, slong prec);
void _acb_dft_cyc_init(acb_dft_cyc_t t, slong dv, slong len, slong prec);

ACB_DFT_INLINE void
acb_dft_cyc_init(acb_dft_cyc_t t, slong len, slong prec)
{
    _acb_dft_cyc_init(t, 1, len, prec);
}

void acb_dft_cyc_clear(acb_dft_cyc_t t);

void _acb_dft_pol_init(acb_dft_pol_t pol, slong dv, acb_ptr z, slong dz, slong len, slong prec);

ACB_DFT_INLINE void
acb_dft_pol_init(acb_dft_pol_t pol, slong len, slong prec)
{
    _acb_dft_pol_init(pol, 1, NULL, 0, len, prec);
}

ACB_DFT_INLINE void
acb_dft_pol_clear(acb_dft_pol_t pol)
{
    if (pol->zclear)
        _acb_vec_clear(pol->z, pol->n);
}

ACB_DFT_INLINE void
acb_dft_rad2_init(acb_dft_rad2_t t, int e, slong prec)
{
    t->e = e;
    t->n = 1 << e;
    t->nz = t->n >> 1;
    t->z = _acb_vec_init(2 * t->nz);
    /* set n/2 roots of order n */
    /* xxx: don't compute twice the number */
    _acb_vec_unit_roots(t->z, t->n, prec);
}

ACB_DFT_INLINE void
acb_dft_rad2_clear(acb_dft_rad2_t t)
{
    _acb_vec_clear(t->z, 2 * t->nz);
}

void acb_dft_bluestein_init(acb_dft_bluestein_t t, slong n, slong prec);

ACB_DFT_INLINE void
acb_dft_bluestein_clear(acb_dft_bluestein_t t)
{
    _acb_vec_clear(t->z, t->n);
    acb_dft_rad2_clear(t->rad2);
}

void _acb_dft_crt_init(acb_dft_crt_t crt, slong dv, slong len, slong prec);

ACB_DFT_INLINE void
acb_dft_crt_init(acb_dft_crt_t crt, slong len, slong prec)
{
    crt->n = len;
    crt_init(crt->c, len);
    crt->dv = 1;
    crt->cyc = _acb_dft_steps_prod(crt->c->m, crt->c->num, prec);
}

ACB_DFT_INLINE void
acb_dft_crt_clear(acb_dft_crt_t crt)
{
    flint_free(crt->cyc);
}

/* utils, could be moved elsewhere */

ACB_DFT_INLINE void
acb_swap_ri(acb_t x)
{
    arb_swap(acb_realref(x), acb_imagref(x));
}

ACB_DFT_INLINE void
acb_vec_swap_ri(acb_ptr v, slong len)
{
    slong k;
    for (k = 0; k < len; k++)
        acb_swap_ri(v + k);
}

ACB_DFT_INLINE void
_acb_vec_kronecker_mul(acb_ptr z, acb_srcptr x, acb_srcptr y, slong len, slong prec)
{
    slong k;
    for (k = 0; k < len; k++)
        acb_mul(z + k, x + k, y + k, prec);
}

/* z[k] = conj(x[k])*y[k] */
ACB_DFT_INLINE void
acb_vec_kronecker_mul_conj(acb_ptr z, acb_srcptr x, acb_srcptr y, slong len, slong prec)
{
    slong k;
    for (k = 0; k < len; k++)
    {
        acb_conj(z + k, x + k);
        acb_mul(z + k, z + k, y + k, prec);
    }
}

ACB_DFT_INLINE void
acb_vec_printd_index(acb_srcptr vec, slong len, slong digits)
{
    slong i;
    for (i = 0; i < len; i++)
        flint_printf("[%ld] ",i), acb_printd(vec + i, digits), flint_printf("\n");
}

#ifdef __cplusplus
}
#endif

#endif