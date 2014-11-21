//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "minerule/Utils/MineruleErrors.hpp"
#include <assert.h>

namespace minerule {

const char* MineruleErrorsNames[] = {
  "MR_ERROR_NO_ERROR",
  "MR_ERROR_UNKNOWN",
  "MR_ERROR_INTERNAL",
  "MR_ERROR_INPUT_FILE_NOT_FOUND",
  "MR_ERROR_OUTPUT_FILE_PROBLEM",
  "MR_ERROR_NO_MINERULE_SPECIFIED",
  "MR_ERROR_NO_OPTIONFILE_SPECIFIED",
  "MR_ERROR_OPTION_CONFIGURATION",
  "MR_ERROR_MINERULE_ALREADY_EXISTS",
  "MR_ERROR_DATABASE_ERROR",
  "MR_ERROR_OPTION_PARSING",
  "MR_ERROR_MINERULETEXT_PARSING",
  "MR_ERROR_CATALOGUE_ERROR",
  "MR_ERROR_OPTIMIZER_ERROR",
  "MR_ERROR_INSTALLATION_PROBLEM",
  "MR_ERROR_SAFETY_PROBLEM"
};

const char* me_error_name(MineruleErrors me) {
  assert( me>=me_error_begin() && me<me_error_end() );
  return MineruleErrorsNames[me];
}

int me_error_begin() {
  return MR_ERROR_NO_ERROR;
}

int me_error_end() {
  return (int)MR_ERROR_SAFETY_PROBLEM + 1;
}


} // namespace
