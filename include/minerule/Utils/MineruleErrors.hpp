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
#ifndef __MINERULEERRORS_H__
#define __MINERULEERRORS_H__

namespace minerule {
  typedef enum {
    MR_ERROR_NO_ERROR=0,
    MR_ERROR_UNKNOWN,
    MR_ERROR_INTERNAL,
    MR_ERROR_INPUT_FILE_NOT_FOUND,
    MR_ERROR_OUTPUT_FILE_PROBLEM,
    MR_ERROR_NO_MINERULE_SPECIFIED,
    MR_ERROR_NO_OPTIONFILE_SPECIFIED,
    MR_ERROR_OPTION_CONFIGURATION,
    MR_ERROR_MINERULE_ALREADY_EXISTS,
    MR_ERROR_DATABASE_ERROR,
    MR_ERROR_OPTION_PARSING,
    MR_ERROR_MINERULETEXT_PARSING,
    MR_ERROR_CATALOGUE_ERROR,
    MR_ERROR_OPTIMIZER_ERROR,
    MR_ERROR_INSTALLATION_PROBLEM,
    MR_ERROR_SAFETY_PROBLEM,
  } MineruleErrors;
  
  const char* me_error_name(MineruleErrors me);
  int me_error_begin();
  int me_error_end();
}





#endif

