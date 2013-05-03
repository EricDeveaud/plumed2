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
#include "blas/blas.h"
#include "lapack.h"
#include "lapack_limits.h"

#include "simple.h"

#include "blas/blas.h"
namespace PLMD{
namespace lapack{
using namespace blas;
void
PLUMED_BLAS_F77_FUNC(slasq1,SLASQ1)(int *n,
	float *d,
	float *e,
	float *work,
	int *info)
{
  float sigmx = 0.0;
  int i,j,k,iinfo;
  float minval,safemin;
  float dtemp,scale;
  float eps;

  eps = PLUMED_GMX_FLOAT_EPS;
  minval = PLUMED_GMX_FLOAT_MIN;
  safemin = minval*(1.0+PLUMED_GMX_FLOAT_EPS);
  *info = 0;

  if(*n<0) {
    *info = -2;
    return;
  }
  
  for(i=0;i<*n-1;i++) {
    d[i] = fabs(d[i]);
    dtemp = fabs(e[i]);
    if(dtemp>sigmx)
      sigmx=dtemp;
  }
  d[*n-1] = fabs(d[*n-1]);
  
  if(fabs(sigmx)<PLUMED_GMX_FLOAT_MIN) {
    PLUMED_BLAS_F77_FUNC(slasrt,SLASRT)("D",n,d,&iinfo);
    return;
  }

  for(i=0;i<*n;i++) {
    if(d[i]>sigmx)
      sigmx=d[i];
  }

  /* Copy d and e into work (z format) and scale.
   * Squaring input data makes scaling by a power of the
   * radix pointless.
   */
  scale = sqrt(eps/safemin);
  i = 1;
  j = 2;
  PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(n,d,&i,work,&j);
  k = *n-1;
  PLUMED_BLAS_F77_FUNC(scopy,SCOPY)(&k,e,&i,work+1,&j);
  i = 0;
  j = 2*(*n)-1;
  k = 1;
  PLUMED_BLAS_F77_FUNC(slascl,SLASCL)("G",&i,&i,&sigmx,&scale,&j,&k,work,&j,&iinfo);


  /* Compute q and e elements */
  for(i=0;i<2*(*n)-1;i++)
    work[i] = work[i]*work[i];

  work[2*(*n)-1] = 0.0;

  PLUMED_BLAS_F77_FUNC(slasq2,SLASQ2)(n,work,info);

  j = 0;
  k = 1;
  if(*info==0) {
    for(i=0;i<*n;i++)
      d[i]=sqrt(work[i]);
    PLUMED_BLAS_F77_FUNC(slascl,SLASCL)("G",&j,&j,&scale,&sigmx,n,&k,d,n,&iinfo);
  }
  return;
}
}
}
