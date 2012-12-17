#include "minerule/Utils/OptionParserLib.h"
#include "Utils/OptionParser_lexer.cpp"

//extern void OPparse();
//extern FILE* OPin;
//struct YY_BUFFER_STATE;

void OP_switch_to_buffer( YY_BUFFER_STATE new_buffer );
void OP_delete_buffer( YY_BUFFER_STATE buffer );
YY_BUFFER_STATE OP_scan_string(const char *str);
namespace minerule {


  std::vector<MineruleOptions::OptionBase*> context;// this maintain the context, i.e., it
                                   // sais to which "Option Class" new options
                                   // belong

  void initializeOptionsFromFile(MineruleOptions& mrOpts, FILE* file) {
    YY_BUFFER_STATE buf = yy_new_buffer(file,YY_BUF_SIZE);
    OP_switch_to_buffer(buf);
    context.push_back(&mrOpts);
    OPparse();
    OP_delete_buffer(buf);
  }

  void initializeOptionsFromString(MineruleOptions& mrOpts,std::string str) {
    YY_BUFFER_STATE buf = OP_scan_string(str.c_str());
    context.push_back(&mrOpts);
    OPparse();
    OP_delete_buffer(buf);
  }

  void pushOptionClassIntoContext(const std::string& oclass) throw(MineruleException){
    /* Here we have to push_back OptionBase pointers */
    assert(context.size()>0);
    context.push_back( &context.back()->subclassForName(oclass) );
  }

  void popOptionClassFromContext() {
    assert(context.size()>=1);
    context.pop_back();
  }

  void setOption(const std::string& name, const std::string& value) {
    assert(context.size()>=1);

    context.back()->setOption(name,value);
  }

  
 








} // namespace
