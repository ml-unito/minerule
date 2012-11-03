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
