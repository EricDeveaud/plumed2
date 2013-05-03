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
#include "lapack.h"
#include "lapack_limits.h"

#include "simple.h"

#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;
void 
PLUMED_BLAS_F77_FUNC(dlasv2,DLASV2)(double *f, 
                        double *g, 
                        double *h__, 
                        double *ssmin, 
                        double *ssmax, 
                        double *snr, 
                        double *csr, 
                        double *snl, 
                        double *csl)
{
    double d__1;

    double a, d__, l, m, r__, s, t, fa, ga, ha, ft, gt, ht, mm, tt,
	     clt, crt, slt, srt;
    int pmax;
    double temp;
    int swap;
    double tsign=1.0;
    int gasmal;

    ft = *f;
    fa = fabs(ft);
    ht = *h__;
    ha = fabs(*h__);

    pmax = 1;
    swap = ha > fa;
    if (swap) {
	pmax = 3;
	temp = ft;
	ft = ht;
	ht = temp;
	temp = fa;
	fa = ha;
	ha = temp;

    }
    gt = *g;
    ga = fabs(gt);
    if (fabs(ga)<PLUMED_GMX_DOUBLE_MIN) {

	*ssmin = ha;
	*ssmax = fa;
	clt = 1.;
	crt = 1.;
	slt = 0.;
	srt = 0.;
    } else {
	gasmal = 1;
	if (ga > fa) {
	    pmax = 2;
	    if (fa / ga < PLUMED_GMX_DOUBLE_EPS) {

		gasmal = 0;
		*ssmax = ga;
		if (ha > 1.) {
		    *ssmin = fa / (ga / ha);
		} else {
		    *ssmin = fa / ga * ha;
		}
		clt = 1.;
		slt = ht / gt;
		srt = 1.;
		crt = ft / gt;
	    }
	}
	if (gasmal) {

	    d__ = fa - ha;
	    if ( fabs( fa - d__ )<PLUMED_GMX_DOUBLE_EPS*fabs( fa + d__ )) {
		l = 1.;
	    } else {
		l = d__ / fa;
	    }

	    m = gt / ft;
	    t = 2. - l;

	    mm = m * m;
	    tt = t * t;
	    s = sqrt(tt + mm);

	    if ( fabs(l)<PLUMED_GMX_DOUBLE_MIN) {
		r__ = fabs(m);
	    } else {
		r__ = sqrt(l * l + mm);
	    }
	    a = (s + r__) * .5;

	    *ssmin = ha / a;
	    *ssmax = fa * a;
	    if ( fabs(mm)<PLUMED_GMX_DOUBLE_MIN) {

		if (fabs(l)<PLUMED_GMX_DOUBLE_MIN) {
		    t = ( (ft>0) ? 2.0 : -2.0) * ( (gt>0) ? 1.0 : -1.0);
		} else {
		    t = gt / ( (ft>0) ? d__ : d__) + m / t;
		}
	    } else {
		t = (m / (s + t) + m / (r__ + l)) * (a + 1.);
	    }
	    l = sqrt(t * t + 4.);
	    crt = 2. / l;
	    srt = t / l;
	    clt = (crt + srt * m) / a;
	    slt = ht / ft * srt / a;
	}
    }
    if (swap) {
	*csl = srt;
	*snl = crt;
	*csr = slt;
	*snr = clt;
    } else {
	*csl = clt;
	*snl = slt;
	*csr = crt;
	*snr = srt;
    }

    if (pmax == 1) {
	tsign = ( (*csr>0) ? 1.0 : -1.0) * ( (*csl>0) ? 1.0 : -1.0) * ( (*f>0) ? 1.0 : -1.0);
    }
    if (pmax == 2) {
	tsign = ( (*snr>0) ? 1.0 : -1.0) * ( (*csl>0) ? 1.0 : -1.0) * ( (*g>0) ? 1.0 : -1.0);
    }
    if (pmax == 3) {
	tsign = ( (*snr>0) ? 1.0 : -1.0) * ( (*snl>0) ? 1.0 : -1.0) * ( (*h__>0) ? 1.0 : -1.0);
    }
    if(tsign<0)
      *ssmax *= -1.0;
    d__1 = tsign * ( (*f>0) ? 1.0 : -1.0) * ( (*h__>0) ? 1.0 : -1.0);
    if(d__1<0)
      *ssmin *= -1.0;
    return;

}
}
}
