//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Dino Ienco (dino.ienco@teledetection.fr)
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
#include "minerule/Algorithms/FSTreeSequence.hpp"

#include <iostream>
#include "minerule/Database/SourceRowColumnIds.hpp"
#include "minerule/Database/SourceRow.hpp"
#include "minerule/Utils/MineruleLogs.hpp"



namespace minerule {

  FSTreeSequence::ItemVector* FSTreeSequence::getFSTreeSequence(){
#warning CONTROLLARE CHE LA COPIA SIA VERAMENTE UTILE (NON SI PUO SEMPLICEMENTE RESTITUIRE seq?).
    return new ItemVector(*seq);
  }

  FSTreeSequence::~FSTreeSequence(){	
    delete seq;
  }

  void FSTreeSequence::add(const ItemType& s){
    seq->push_back(s);
  }

  FSTreeSequence::FSTreeSequence(const FSTreeSequence &in, size_t start, size_t n_item){
    seq= new ItemVector();
    ItemVector::const_iterator it = in.seq->begin();

    for(size_t i=0; i<start; ++i) {
      ++it;
    }


    for (size_t i=start;(i<in.seq->size())&&(i<start+n_item);++i) {
      seq->push_back( *it );
      ++it;
    }
  }

  void FSTreeSequence::stampa(){
    ItemVector::const_iterator it;
    for(it=seq->begin(); it!=seq->end(); ++it) {
      MRLog() << *it << std::endl;
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

 std::string FSTreeSequence::toStdString(){
    ItemVector::const_iterator it;
   std::string ris="";
    for (it=seq->begin();it!=seq->end();it++)
      ris = ris+" "+(*it).getSQLData();
    
    /*   std::string ris="";
    if (seq->size()>0)
      ris=(*seq)[0];
    for (size_t i=1;i<seq->size();i++)
    ris=ris+"->"+(*seq)[i]; */
    return ris;
  }

  void FSTreeSequence::svuota(){
    delete seq;
    seq=new ItemVector();
  }

  bool FSTreeSequence::operator==(FSTreeSequence& s){
    return *seq==*s.seq;
    /*

    if (seq->size()!=s.size())
      return false;

    ItemVector* vec = s.getFSTreeSequence();
    for (size_t i=0; i < vec->size();i++)
      if ((*vec)[i]!=(*seq)[i]){
	delete vec;
	return false;
      }
    delete vec;
    return true; */

  }

  void FSTreeSequence::read(mrdb::ResultSet* rs, const SourceRowColumnIds& rowDes){
	SourceRow hbsr(rs,rowDes);
	ItemType gid;
	if(!rs->isAfterLast()) 
		gid = hbsr.getGroup();
	while (!rs->isAfterLast() && gid == hbsr.getGroup()) {
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
/*    ItemVector* vec1 =seq;
    ItemVector* vec2 = in.seq;
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
