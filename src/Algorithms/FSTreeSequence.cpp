#include "Algorithms/FSTreeSequence.h"

#include <iostream>
#include "MRDatabase/sourcerowcommon.h"
#include "MRDatabase/sourcerow.h"
#include "Utils/MineruleLogs.h"

using namespace std;

namespace minerule {

  FSTreeSequence::ListType* FSTreeSequence::getFSTreeSequence(){
#warning CONTROLLARE CHE LA COPIA SIA VERAMENTE UTILE (NON SI PUO SEMPLICEMENTE RESTITUIRE seq?).
    return new ListType(*seq);
  }

  FSTreeSequence::~FSTreeSequence(){	
    delete seq;
  }

  void FSTreeSequence::add(const ItemType& s){
    seq->push_back(s);
  }

  FSTreeSequence::FSTreeSequence(const FSTreeSequence &in, size_t start, size_t n_item){
    seq= new ListType();
    ListType::const_iterator it = in.seq->begin();

    for(size_t i=0; i<start; ++i) {
      ++it;
    }


    for (size_t i=start;(i<in.seq->size())&&(i<start+n_item);++i) {
      seq->push_back( *it );
      ++it;
    }
  }

  void FSTreeSequence::stampa(){
    ListType::const_iterator it;
    for(it=seq->begin(); it!=seq->end(); ++it) {
      MRLog() << *it << endl;
    }
  } 
  
  size_t FSTreeSequence::size(){
    return seq->size();
  }

   ItemType FSTreeSequence::removeHead(){
    ItemType res = seq->front();
    seq->erase(seq->begin());
    return res;
  }

  void FSTreeSequence::insertHead(const ItemType& s){
    seq->insert(seq->begin(),s);
  }

  string FSTreeSequence::toStdString(){
    ListType::const_iterator it;
    string ris="";
    for (it=seq->begin();it!=seq->end();it++)
      ris = ris+" "+(*it).getSQLData();
    
    /*    string ris="";
    if (seq->size()>0)
      ris=(*seq)[0];
    for (size_t i=1;i<seq->size();i++)
    ris=ris+"->"+(*seq)[i]; */
    return ris;
  }

  void FSTreeSequence::svuota(){
    delete seq;
    seq=new ListType();
  }

  bool FSTreeSequence::operator==(FSTreeSequence& s){
    return *seq==*s.seq;
    /*

    if (seq->size()!=s.size())
      return false;

    ListType* vec = s.getFSTreeSequence();
    for (size_t i=0; i < vec->size();i++)
      if ((*vec)[i]!=(*seq)[i]){
	delete vec;
	return false;
      }
    delete vec;
    return true; */

  }

  void FSTreeSequence::read(odbc::ResultSet* rs, const HeadBodySourceRowDescription& rowDes){
	HeadBodySourceRow hbsr(rs,rowDes);
	ItemType gid;
	if(!rs->isAfterLast()) 
		gid = hbsr.getGroupBody();
	while (!rs->isAfterLast() && gid == hbsr.getGroupBody()) {
		seq->push_back( hbsr.getBody() );

		rs->next();
		if(!rs->isAfterLast())
			hbsr.init(rs,rowDes);
	}
  }
  
  bool FSTreeSequence::operator<(const FSTreeSequence& in) const {
    return lexicographical_compare(seq->begin(),
				   seq->end(),
				   in.seq->begin(),
				   in.seq->end());
/*    ListType* vec1 =seq;
    ListType* vec2 = in.seq;
    if (vec1->size()<vec2->size())
      return true;
    if (vec1->size()>vec2->size())
      return false;
			
    size_t i=0;
    for (;i<vec1->size()&&((*vec1)[i]==(*vec2)[i]);i++);
    // for senza corpo
			
    if (i==vec1->size()) return false;
    else return (*vec1)[i]<(*vec2)[i]; */
  }


} // namespace
