/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
These files are semi-automatic translations by f2c from the original netlib LAPACK library.
The source has been modified to (mostly) use modern C formatting, and to get rid of
compiler warnings. Any errors in doing this should be blamed on the Gromacs developers, and
not the reference LAPACK implementation.

The reference LAPACK implementation is available from http://www.netlib.org/lapack 

LAPACK does not come with a formal named "license", but a general statement saying:

"The reference LAPACK is a freely-available software package. It is available from netlib
via anonymous ftp and the World Wide Web. Thus, it can be included in commercial software
packages (and has been). We only ask that proper credit be given to the authors."

While the rest of Gromacs is GPL, we think it's only fair to give you the same rights to
our modified LAPACK files as the original netlib versions, so do what you want with them.

However, be warned that we have only tested that they to the right thing in the cases used
in Gromacs (primarily full & sparse matrix diagonalization), so in most cases it is a much
better idea to use the full reference implementation.

Erik Lindahl, 2008-10-07.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2012,2013, by the GROMACS development team, led by
 * David van der Spoel, Berk Hess, Erik Lindahl, and including many
 * others, as listed in the AUTHORS file in the top-level source
 * directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */
#include <math.h>
#include "simple.h"

#include "blas/blas.h"
#include "lapack.h"
#include "lapack_limits.h"

#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;
void 
PLUMED_BLAS_F77_FUNC(dlasd7,DLASD7)(int *icompq, 
	int *nl, 
	int *nr, 
	int *sqre, 
	int *k, 
	double *d__, 
	double *z__, 
	double *zw, 
	double *vf, 
	double *vfw,
	double *vl, 
	double *vlw,
	double *alpha, 
	double *beta,
	double *dsigma, 
	int *idx, 
	int *idxp,
	int *idxq, 
	int *perm, 
	int *givptr,
	int *givcol, 
	int *ldgcol, 
	double *givnum,
	int *ldgnum, 
	double *c__, 
	double *s, 
	int *info)
{
    int givcol_dim1, givcol_offset, givnum_dim1, givnum_offset, i__1;
    double d__1, d__2;

    int i__, j, m, n, k2;
    double z1;
    int jp;
    double eps, tau, tol;
    int nlp1, nlp2, idxi, idxj;
    int idxjp;
    int jprev = 0;
    double hlftol;
    int c__1 = 1;

    --d__;
    --z__;
    --zw;
    --vf;
    --vfw;
    --vl;
    --vlw;
    --dsigma;
    --idx;
    --idxp;
    --idxq;
    --perm;
    givcol_dim1 = *ldgcol;
    givcol_offset = 1 + givcol_dim1;
    givcol -= givcol_offset;
    givnum_dim1 = *ldgnum;
    givnum_offset = 1 + givnum_dim1;
    givnum -= givnum_offset;

    *info = 0;
    n = *nl + *nr + 1;
    m = n + *sqre;

    nlp1 = *nl + 1;
    nlp2 = *nl + 2;
    if (*icompq == 1) {
	*givptr = 0;
    }

    z1 = *alpha * vl[nlp1];
    vl[nlp1] = 0.;
    tau = vf[nlp1];
    for (i__ = *nl; i__ >= 1; --i__) {
	z__[i__ + 1] = *alpha * vl[i__];
	vl[i__] = 0.;
	vf[i__ + 1] = vf[i__];
	d__[i__ + 1] = d__[i__];
	idxq[i__ + 1] = idxq[i__] + 1;
    }
    vf[1] = tau;

    i__1 = m;
    for (i__ = nlp2; i__ <= i__1; ++i__) {
	z__[i__] = *beta * vf[i__];
	vf[i__] = 0.;
    }
    i__1 = n;
    for (i__ = nlp2; i__ <= i__1; ++i__) {
	idxq[i__] += nlp1;
    }

    i__1 = n;
    for (i__ = 2; i__ <= i__1; ++i__) {
	dsigma[i__] = d__[idxq[i__]];
	zw[i__] = z__[idxq[i__]];
	vfw[i__] = vf[idxq[i__]];
	vlw[i__] = vl[idxq[i__]];
    }

    PLUMED_BLAS_F77_FUNC(dlamrg,DLAMRG)(nl, nr, &dsigma[2], &c__1, &c__1, &idx[2]);

    i__1 = n;
    for (i__ = 2; i__ <= i__1; ++i__) {
	idxi = idx[i__] + 1;
	d__[i__] = dsigma[idxi];
	z__[i__] = zw[idxi];
	vf[i__] = vfw[idxi];
	vl[i__] = vlw[idxi];
    }

    eps = PLUMED_GMX_DOUBLE_EPS;

    d__1 = fabs(*alpha);
    d__2 = fabs(*beta);
    tol = (d__1>d__2) ? d__1 : d__2;
    d__2 = fabs(d__[n]);
    tol = eps * 64. * ((d__2>tol) ? d__2 : tol);

    *k = 1;
    k2 = n + 1;
    i__1 = n;
    for (j = 2; j <= i__1; ++j) {
	if (fabs(z__[j]) <= tol) {

	    --k2;
	    idxp[k2] = j;
	    if (j == n) {
		goto L100;
	    }
	} else {
	    jprev = j;
	    goto L70;
	}
    }
L70:
    j = jprev;
L80:
    ++j;
    if (j > n) {
	goto L90;
    }
    if (fabs(z__[j]) <= tol) {

	--k2;
	idxp[k2] = j;
    } else {

	if (fabs(d__[j] - d__[jprev]) <= tol) {

	    *s = z__[jprev];
	    *c__ = z__[j];

	    tau = PLUMED_BLAS_F77_FUNC(dlapy2,DLAPY2)(c__, s);
	    z__[j] = tau;
	    z__[jprev] = 0.;
	    *c__ /= tau;
	    *s = -(*s) / tau;


	    if (*icompq == 1) {
		++(*givptr);
		idxjp = idxq[idx[jprev] + 1];
		idxj = idxq[idx[j] + 1];
		if (idxjp <= nlp1) {
		    --idxjp;
		}
		if (idxj <= nlp1) {
		    --idxj;
		}
		givcol[*givptr + (givcol_dim1 << 1)] = idxjp;
		givcol[*givptr + givcol_dim1] = idxj;
		givnum[*givptr + (givnum_dim1 << 1)] = *c__;
		givnum[*givptr + givnum_dim1] = *s;
	    }
	    PLUMED_BLAS_F77_FUNC(drot,DROT)(&c__1, &vf[jprev], &c__1, &vf[j], &c__1, c__, s);
	    PLUMED_BLAS_F77_FUNC(drot,DROT)(&c__1, &vl[jprev], &c__1, &vl[j], &c__1, c__, s);
	    --k2;
	    idxp[k2] = jprev;
	    jprev = j;
	} else {
	    ++(*k);
	    zw[*k] = z__[jprev];
	    dsigma[*k] = d__[jprev];
	    idxp[*k] = jprev;
	    jprev = j;
	}
    }
    goto L80;
L90:

    ++(*k);
    zw[*k] = z__[jprev];
    dsigma[*k] = d__[jprev];
    idxp[*k] = jprev;

L100:

    i__1 = n;
    for (j = 2; j <= i__1; ++j) {
	jp = idxp[j];
	dsigma[j] = d__[jp];
	vfw[j] = vf[jp];
	vlw[j] = vl[jp];
    }
    if (*icompq == 1) {
	i__1 = n;
	for (j = 2; j <= i__1; ++j) {
	    jp = idxp[j];
	    perm[j] = idxq[idx[jp] + 1];
	    if (perm[j] <= nlp1) {
		--perm[j];
	    }
	}
    }
    i__1 = n - *k;
    PLUMED_BLAS_F77_FUNC(dcopy,DCOPY)(&i__1, &dsigma[*k + 1], &c__1, &d__[*k + 1], &c__1);

    dsigma[1] = 0.;
    hlftol = tol / 2.;
    if (fabs(dsigma[2]) <= hlftol) {
	dsigma[2] = hlftol;
    }
    if (m > n) {
	z__[1] = PLUMED_BLAS_F77_FUNC(dlapy2,DLAPY2)(&z1, &z__[m]);
	if (z__[1] <= tol) {
	    *c__ = 1.;
	    *s = 0.;
	    z__[1] = tol;
	} else {
	    *c__ = z1 / z__[1];
	    *s = -z__[m] / z__[1];
	}
	PLUMED_BLAS_F77_FUNC(drot,DROT)(&c__1, &vf[m], &c__1, &vf[1], &c__1, c__, s);
	PLUMED_BLAS_F77_FUNC(drot,DROT)(&c__1, &vl[m], &c__1, &vl[1], &c__1, c__, s);
    } else {
	if (fabs(z1) <= tol) {
	    z__[1] = tol;
	} else {
	    z__[1] = z1;
	}
    }

    i__1 = *k - 1;
    PLUMED_BLAS_F77_FUNC(dcopy,DCOPY)(&i__1, &zw[2], &c__1, &z__[2], &c__1);
    i__1 = n - 1;
    PLUMED_BLAS_F77_FUNC(dcopy,DCOPY)(&i__1, &vfw[2], &c__1, &vf[2], &c__1);
    i__1 = n - 1;
    PLUMED_BLAS_F77_FUNC(dcopy,DCOPY)(&i__1, &vlw[2], &c__1, &vl[2], &c__1);

    return;

}


}
}
