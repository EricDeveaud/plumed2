/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2014-2019 The plumed team
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
#include "core/ActionShortcut.h"

namespace PLMD {
namespace vatom {

class COM : public ActionShortcut {
public:
  static void registerKeywords( Keywords& keys );
  explicit COM(const ActionOptions&);
};

PLUMED_REGISTER_ACTION(COM,"COM")

void COM::registerKeywords( Keywords& keys ) {
  ActionShortcut::registerKeywords( keys );
}

COM::COM(const ActionOptions& ao) :
Action(ao),
ActionShortcut(ao)
{
  readInputLine( getShortcutLabel() + ": CENTER WEIGHTS=@masses " + convertInputLineToString() );
}

}
}
