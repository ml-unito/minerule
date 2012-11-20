#include "Algorithms/CCSMSequence.h"

#include "Database/SourceRowColumnIds.h"
#include "Database/SourceRow.h"

#include "time.h"


namespace minerule{
	
	void CCSMSequence::read(odbc::ResultSet* rs, const SourceRowColumnIds& rowDes){
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
  
	
  /*
	void CCSMSequence::read(const std::string& s){	
	std::string empty("");
		size_t i=0;
		while (s[i]==' ')
			i++;
		if (i==s.size())
			return;
	std::string temp("");
		for (;i<s.size();i++){
			if (s[i]==' '){
				if (temp!=empty){
					seq->push_back(temp);
				}
				temp="";
			}else {
				temp=temp+s[i];
			}
		}
		if (temp!=empty)
			seq->push_back(temp);

	}
  */
	bool CCSMSequence::operator<(const CCSMSequence& in){
		return lexicographical_compare(seq->begin(),	
					       seq->end(),	
					       in.seq->begin(),	
					       in.seq->end());

	};

	bool CCSMSequence::operator==(const CCSMSequence& in){
		bool first=
			lexicographical_compare(seq->begin(),	
					       seq->end(),	
					       in.seq->begin(),	
					       in.seq->end());
		bool second=
			lexicographical_compare(in.seq->begin(),	
					        in.seq->end(),	
					        seq->begin(),	
					        seq->end());
		return (!first)&&(!second);

	};

	//il second e il singleton
	//il first e la sequenza da estendere
	CCSMSequence* CCSMSequence::merge(CCSMSequence* first, CCSMSequence* second, int min_gap ,int max_gap, double threshold){
		
		CCSMSequence* ris= new CCSMSequence(*first);//=first;
		ris->setLastItem(second);
		//cout<<"ecco il LAST ITEM"<<std::endl;
		//cout<<ris->getLastItem()->toStdString()<<std::endl;
		//List_Type::iterator it;
		/*
		for (it = second->seq->begin();it!=second->seq->end();it++)	 
			ris->seq->push_back(*it);
		*/
		ris->seq->push_back((*second->seq)[0]);
		
		ris->count=0;
		BitVector::iterator it_ris, it_second;
		it_ris=ris->listSid.begin();
		it_second = second->listSid.begin();
		//////////////////////////////////
/*
		for (size_t i=0;i<first->listSid.size();i++,++it_second,++it_ris){
			bool ris_i =(*it_ris)&(*it_second);
			(*it_ris)=ris_i;
			if (ris_i)	
				ris->count++;
		}
		if (ris->count<threshold){
			//	std::cout<<"ESCO 1"<<std::endl;
				return ris;
		}
		/////////////////////////////////

		it_ris=ris->listSid.begin();
		it_second = second->listSid.begin();
		ris->count=0;
*/
		for (size_t i=0;i<first->listSid.size();i++,++it_second,++it_ris){
			bool ris_i =(*it_ris)&(*it_second);
			(*it_ris)=ris_i;

			Eid_List *f=first->eid_vector[i];
			Eid_List *s=second->eid_vector[i];
			if (ris_i && (*s)[s->size()-1].first > (*f)[0].second ){
				ris->count++;
			}else 
				(*it_ris)=ABSENT;
		}
		
		if (ris->count<threshold){
			//	std::cout<<"ESCO 2"<<std::endl;
				return ris;
		}

		it_ris=ris->listSid.begin();
		bool canContinue=true;
		for (size_t i=0;i<first->listSid.size()&&canContinue;++i,++it_ris){
			Eid_List* event_id;
			if ((*it_ris)){//se la sequenza è presente allora calcolo intersezione tra le event_list
				event_id=CCSMSequence::mergeEidRtoLeft(*first->eid_vector[i],*second->eid_vector[i],min_gap,max_gap);
			}
			else 
				event_id=new Eid_List();	
			//CORREZIONE DELla lista dei SID per tener conto dell ordine
			//siccome se second appare prima di first il sid per quella transazione e messo ad uno erroneamente
			//xcio lo correggo controllando se sono stati generati event_id
			//se non sono stati generati setto il listSid[i] ad ABSENT ed incremento count solo quando ho degli event_id
			
			if ((*it_ris) && event_id->size()==0){
				(*it_ris)=ABSENT;
				ris->count--;
			}
			
			if (ris->count<threshold){
				canContinue=false;
			}
			ris->eid_vector.push_back(event_id);
		}
		//cout<<"fusioni di event list "<<cc<<std::endl;
		return ris;

	};	

	



	//in questo caso il second è il singleton che cerchiamo di aggiungere alla sequenza
	//stiamo estendendo la sequenza da sinistra verso destra, aggiungendo al fondo un elemento
	std::vector<std::pair<int,int> >* CCSMSequence::mergeEidRtoLeft(const Eid_List& first, const Eid_List& second, int min_gap  , int max_gap){	
	Eid_List* ris= new Eid_List();
	//ris->reserve(10000);
	int last_eid_seq;
	int eid_singleton;
	int first_eid_seq;
	Eid_List::const_iterator it_first, it_second;
	it_first = first.begin();
	if (second[second.size()-1].first<=(*it_first).second){
		//cout<<"escamotage"<<std::endl;
		return ris;
	}	
	bool continue_f=true;
	bool continue_s=true;

	for (it_first=first.begin();it_first!=first.end()&&continue_f;++it_first){

		last_eid_seq=(*it_first).second;
		first_eid_seq=(*it_first).first;
		
		if (second[second.size()-1].first>last_eid_seq )
		{	continue_s=true;
			
	
			for (it_second=second.begin();it_second!=second.end()&&continue_s;++it_second){
				eid_singleton=(*it_second).first;
				int diff_event= eid_singleton-last_eid_seq;
				//se l'ultimo elemento analizzato della Eid_List dell elemento con cui si estende in coda
				//e in posizione > max_gap+1 rispetto all'elemento della Eid_List del primo elemento
				//si smette di analizzare gli elementi del secondo elemento siccome non potranno più essere inseriti
				//in coda per generare nuove sequenze
				if ( diff_event > max_gap+1 )
					continue_s=false;
				//se al massimo il numero degli elementi tra l'ultimo elemento della sequenza
				//e il singleton e <= max_gap
				//&&   il singleton nella sequenza compare dopo l'ultimo elemento della sequenza
				//&&   il singleton sia distante almeno mig_gap dalla sequenza presa in considerazione
				if( eid_singleton > last_eid_seq && 
				    eid_singleton-max_gap-1<=last_eid_seq && 
		 		    eid_singleton-last_eid_seq-1>=min_gap){
						//cout<<"inserisco coppia "<<first_eid_seq<<","<<eid_singleton<<std::endl;	
						//cout<<"inserito"<<std::endl;
						ris->push_back(std::pair<int,int>(first_eid_seq,eid_singleton));	
				}			
			}
		}else 	
			continue_f=false;

	}
	return ris;
	}


}// END NAMESPACE
