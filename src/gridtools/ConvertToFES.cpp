/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2015-2019 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed.org for more information.

   This file is part of plumed, version 2.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#include "core/ActionRegister.h"
#include "core/PlumedMain.h"
#include "core/Atoms.h"
#include "function/Function.h"

//+PLUMEDOC GRIDANALYSIS CONVERT_TO_FES
/*
Convert a histogram, \f$H(x)\f$, to a free energy surface using \f$F(x) = -k_B T \ln H(x)\f$.

This action allows you to take a free energy surface that was calculated using the \ref HISTOGRAM
action and to convert it to a free energy surface.  This transformation performed by doing:

\f[
F(x) = -k_B T \ln H(x)
\f]

The free energy calculated on a grid is output by this action and can be printed using \ref DUMPGRID

\par Examples

This is a typical example showing how CONVERT_TO_FES might be used when post processing a trajectory.
The input below calculates the free energy as a function of the distance between atom 1 and atom 2.
This is done by accumulating a histogram as a function of this distance using kernel density estimation
and the HISTOGRAM action.  All the data within this trajectory is used in the construction of this
HISTOGRAM.  Finally, once all the data has been read in, the histogram is converted to a free energy
using the formula above and the free energy is output to a file called fes.dat

\plumedfile
x: DISTANCE ATOMS=1,2
hA1: HISTOGRAM ARG=x GRID_MIN=0.0 GRID_MAX=3.0 GRID_BIN=100 BANDWIDTH=0.1
ff: CONVERT_TO_FES GRID=hA1 TEMP=300
DUMPGRID GRID=ff FILE=fes.dat
\endplumedfile

*/
//+ENDPLUMEDOC

namespace PLMD {
namespace gridtools {

class ConvertToFES : public function::Function {
private:
  double simtemp;
public:
  static void registerKeywords( Keywords& keys );
  explicit ConvertToFES(const ActionOptions&ao);
  void calculateFunction( const std::vector<double>& args, MultiValue& myvals ) const ;
};

PLUMED_REGISTER_ACTION(ConvertToFES,"CONVERT_TO_FES")

void ConvertToFES::registerKeywords( Keywords& keys ) {
  function::Function::registerKeywords( keys ); keys.use("ARG");
  keys.add("optional","TEMP","the temperature at which you are operating");
}

ConvertToFES::ConvertToFES(const ActionOptions&ao):
  Action(ao),
  Function(ao)
{
  simtemp=0.; parse("TEMP",simtemp);
  if(simtemp>0) simtemp*=plumed.getAtoms().getKBoltzmann();
  else simtemp=plumed.getAtoms().getKbT();
  if( simtemp==0 ) error("TEMP not set - use keyword TEMP");

  // Sanity checks on input to convert to FES
  if( getNumberOfArguments()!=1 ) error("should only have one argument");
  if( getPntrToArgument(0)->getRank()==0 || !getPntrToArgument(0)->hasDerivatives() ) error("input should be a grid");

  // And value
  addValueWithDerivatives();
}

void ConvertToFES::calculateFunction( const std::vector<double>& args, MultiValue& myvals ) const {
  addValue(0, -simtemp*std::log(args[0]), myvals );
  if( args[0]>0 ) addDerivative( 0, 0, -(simtemp/args[0]), myvals );
  else addDerivative( 0, 0, 0, myvals );
}

}
}
