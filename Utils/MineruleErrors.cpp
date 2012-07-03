#include "MineruleErrors.h"
#include <assert.h>

namespace minerule {

const char* MineruleErrorsNames[] = {
  "MR_ERROR_NO_ERROR",
  "MR_ERROR_UNKNOWN",
  "MR_ERROR_INTERNAL",
  "MR_ERROR_INPUT_FILE_NOT_FOUND",
  "MR_ERROR_OUTPUT_FILE_PROBLEM",
  "MR_ERROR_NO_MINERULE_SPECIFIED",
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
