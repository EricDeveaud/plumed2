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

#include "lapack.h"
#include "lapack_limits.h"
#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;

void PLUMED_BLAS_F77_FUNC(dlar1vx,DLAR1VX)(int *n, 
	      int *b1, 
	      int *bn,
	      double *sigma, 
	      double *d__, 
	      double *l, 
	      double *ld, 
	      double *lld, 
	      double *eval, 
	      double *gersch, 
	      double *z__, 
	      double *ztz, 
	      double *mingma, 
	      int *r__, 
	      int *isuppz, 
	      double *work)
{
    int i__1;

    int i__, j;
    double s;
    int r1, r2;
    int to;
    double eps, tmp;
    int indp, inds, from;
    double dplus;
    int sawnan;
    int indumn;
    double dminus;

    --work;
    --isuppz;
    --z__;
    --gersch;
    --lld;
    --ld;
    --l;
    --d__;

    /* Function Body */
    eps = PLUMED_GMX_DOUBLE_EPS;
    if (*r__ == 0) {

	r1 = *b1;
	r2 = *bn;
	i__1 = *bn;
	for (i__ = *b1; i__ <= i__1; ++i__) {
	    if (*eval >= gersch[(i__ << 1) - 1] && *eval <= gersch[i__ * 2]) {
		r1 = i__;
		goto L20;
	    }
	}
	goto L40;
L20:
	i__1 = *b1;
	for (i__ = *bn; i__ >= i__1; --i__) {
	    if (*eval >= gersch[(i__ << 1) - 1] && *eval <= gersch[i__ * 2]) {
		r2 = i__;
		goto L40;
	    }
	}
    } else {
	r1 = *r__;
	r2 = *r__;
    }

L40:
    indumn = *n;
    inds = (*n << 1) + 1;
    indp = *n * 3 + 1;
    sawnan = 0;

    if (*b1 == 1) {
	work[inds] = 0.;
    } else {
	work[inds] = lld[*b1 - 1];
    }
    s = work[inds] - *sigma;
    i__1 = r2 - 1;
    for (i__ = *b1; i__ <= i__1; ++i__) {
	dplus = d__[i__] + s;
	work[i__] = ld[i__] / dplus;
	work[inds + i__] = s * work[i__] * l[i__];
	s = work[inds + i__] - *sigma;
    }

    if (! (s > 0. || s < 1.)) {

	sawnan = 1;
	j = *b1 + 1;
L60:
	if (work[inds + j] > 0. || work[inds + j] < 1.) {
	    ++j;
	    goto L60;
	}
	work[inds + j] = lld[j];
	s = work[inds + j] - *sigma;
	i__1 = r2 - 1;
	for (i__ = j + 1; i__ <= i__1; ++i__) {
	    dplus = d__[i__] + s;
	    work[i__] = ld[i__] / dplus;
	    if (fabs(work[i__])<PLUMED_GMX_DOUBLE_MIN) {
		work[inds + i__] = lld[i__];
	    } else {
		work[inds + i__] = s * work[i__] * l[i__];
	    }
	    s = work[inds + i__] - *sigma;
	}
    }

    work[indp + *bn - 1] = d__[*bn] - *sigma;
    i__1 = r1;
    for (i__ = *bn - 1; i__ >= i__1; --i__) {
	dminus = lld[i__] + work[indp + i__];
	tmp = d__[i__] / dminus;
	work[indumn + i__] = l[i__] * tmp;
	work[indp + i__ - 1] = work[indp + i__] * tmp - *sigma;
    }
    tmp = work[indp + r1 - 1];
    if (! (tmp > 0. || tmp < 1.)) {

	sawnan = 1;
	j = *bn - 3;
L90:
	if (work[indp + j] > 0. || work[indp + j] < 1.) {
	    --j;
	    goto L90;
	}
	work[indp + j] = d__[j + 1] - *sigma;
	i__1 = r1;
	for (i__ = j; i__ >= i__1; --i__) {
	    dminus = lld[i__] + work[indp + i__];
	    tmp = d__[i__] / dminus;
	    work[indumn + i__] = l[i__] * tmp;
	    if (fabs(tmp)<PLUMED_GMX_DOUBLE_MIN) {
		work[indp + i__ - 1] = d__[i__] - *sigma;
	    } else {
		work[indp + i__ - 1] = work[indp + i__] * tmp - *sigma;
	    }
	}
    }

    *mingma = work[inds + r1 - 1] + work[indp + r1 - 1];
    if (fabs(*mingma)<PLUMED_GMX_DOUBLE_MIN) {
	*mingma = eps * work[inds + r1 - 1];
    }
    *r__ = r1;
    i__1 = r2 - 1;
    for (i__ = r1; i__ <= i__1; ++i__) {
	tmp = work[inds + i__] + work[indp + i__];
	if (fabs(tmp)<PLUMED_GMX_DOUBLE_MIN) {
	    tmp = eps * work[inds + i__];
	}
	if (fabs(tmp) < fabs(*mingma)) {
	    *mingma = tmp;
	    *r__ = i__ + 1;
	}
    }

    isuppz[1] = *b1;
    isuppz[2] = *bn;
    z__[*r__] = 1.;
    *ztz = 1.;
    if (! sawnan) {
	from = *r__ - 1;
	i__1 = *r__ - 32;
	to = (i__1>(*b1)) ? i__1 : (*b1);
L120:
	if (from >= *b1) {
	    i__1 = to;
	    for (i__ = from; i__ >= i__1; --i__) {
		z__[i__] = -(work[i__] * z__[i__ + 1]);
		*ztz += z__[i__] * z__[i__];
	    }
	    if (fabs(z__[to]) <= eps && fabs(z__[to + 1]) <= eps) {
		isuppz[1] = to + 2;
	    } else {
		from = to - 1;
		i__1 = to - 32;
		to = (i__1>*b1) ? i__1 : *b1;
		goto L120;
	    }
	}
	from = *r__ + 1;
	i__1 = *r__ + 32;
	to = (i__1<*bn) ? i__1 : *bn;
L140:
	if (from <= *bn) {
	    i__1 = to;
	    for (i__ = from; i__ <= i__1; ++i__) {
		z__[i__] = -(work[indumn + i__ - 1] * z__[i__ - 1]);
		*ztz += z__[i__] * z__[i__];
	    }
	    if (fabs(z__[to]) <= eps && fabs(z__[to - 1]) <= eps) {
		isuppz[2] = to - 2;
	    } else {
		from = to + 1;
		i__1 = to + 32;
		to = (i__1<*bn) ? i__1 : *bn;
		goto L140;
	    }
	}
    } else {
	i__1 = *b1;
	for (i__ = *r__ - 1; i__ >= i__1; --i__) {
	    if (fabs(z__[i__ + 1])<PLUMED_GMX_DOUBLE_MIN) {
		z__[i__] = -(ld[i__ + 1] / ld[i__]) * z__[i__ + 2];
	    } else {
		z__[i__] = -(work[i__] * z__[i__ + 1]);
	    }
	    if (fabs(z__[i__]) <= eps && fabs(z__[i__ + 1]) <= eps) {
		isuppz[1] = i__ + 2;
		goto L170;
	    }
	    *ztz += z__[i__] * z__[i__];
	}
L170:
	i__1 = *bn - 1;
	for (i__ = *r__; i__ <= i__1; ++i__) {
	    if (fabs(z__[i__])<PLUMED_GMX_DOUBLE_MIN) {
		z__[i__ + 1] = -(ld[i__ - 1] / ld[i__]) * z__[i__ - 1];
	    } else {
		z__[i__ + 1] = -(work[indumn + i__] * z__[i__]);
	    }
	    if (fabs(z__[i__]) <= eps && fabs(z__[i__ + 1]) <= eps) {
		isuppz[2] = i__ - 1;
		break;
	    }
	    *ztz += z__[i__ + 1] * z__[i__ + 1];
	}
    }

    return;

}


}
}
