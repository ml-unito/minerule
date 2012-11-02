#ifndef CCSMSequence__H
#define CCSMSequence__H
#include "Bit_vector.h"

#include <list>
#include <string>
#include <iostream>
#include "MRDatabase/itemtype.h"
#include "MRDatabase/sourcerowcommon.h"
using namespace std;
#define PRESENT 1
#define ABSENT 0


namespace minerule{

class CCSMSequence {
	
	
public:
	typedef vector<pair<int,int> > Eid_List;
	
private:
	bool singleton;
	//typedef vector<pair<int,int> > Eid_List;
	Bit_vector listSid;
	typedef std::vector<ItemType> List_Type;
	List_Type* seq;
	int count;
	std::vector<Eid_List*> eid_vector;
	
	//puntatore al singleton presente in unltima posizione di questa sequenza
	//utile per velocizzare la generazione di una sequenza di lunghezza k da due sequenze di lunghezza k-1 che condividono k-2 elementi
	// a-b-c e b-c-d condividono b-c e da queste due genererò a-b-c-d come intersezione di a-b-c e d, L(a-b-c) intersect L(d)
	//appunto d è l'ultimo singleton di b-c-d
	CCSMSequence* last;
	list<CCSMSequence*> prefix;
	list<CCSMSequence*> suffix;
	
	static vector<pair<int,int> >* mergeEidRtoLeft(const Eid_List& first, const Eid_List& second, int min_gap ,int max_gap);

public:
		
	typedef pair<vector<ItemType>,int> ResultItems;

	CCSMSequence(CCSMSequence* l=NULL) {
		singleton=false;
		count=0;
		seq = new List_Type();
		last=l;
		listSid.reserve(1000000);
		eid_vector.reserve(1000000);
		/*cout<<"bit_vector.capacity "<<listSid.capacity()<<endl;
		cout<<"bit_vector.max_size "<<listSid.max_size()<<endl;
				cout<<"eid_vector.capacity "<<eid_vector.capacity()<<endl;
		cout<<"eid_vector.max_size "<<eid_vector.max_size()<<endl;
		getchar();*/
	};
		
	CCSMSequence(const CCSMSequence &in, size_t start, size_t n_item, CCSMSequence* l=NULL){
		seq = new List_Type();
		if (n_item==1){
			seq->push_back((*in.seq)[start]);
		}
		else {
			List_Type::iterator it = in.seq->begin();
			for (size_t i=0;i<start;++i){
				++it;
			}
			for (size_t i=start;(i<in.seq->size())&&(i<start+n_item);++i){
				seq->push_back(*it);
				++it;
			}
		}
		count=0;
		if (seq->size()==1)
			singleton=true;
		else singleton=false;
		last=l;
	}

	CCSMSequence(const CCSMSequence &in){
		seq= new List_Type();
		*seq=*in.seq;
		count=0;
		if (seq->size()==1)
			singleton=true;
		else singleton=false;
		listSid = in.listSid;
		last = in.last;
		count=in.count;
	}

	void reduction(vector<size_t>& k){
		//elimino dalla lista degli Eventi gli eventi corrispondenti alle transazioni che non sono utili al fine del conteggio
		//delle frequenze del livello a cui siamo arrivati in modo da migliorare
		//le performance dell'algoritmo e l'occupazione di memoria

		//time_t init,end;
		//time(&init);
		vector<Eid_List*>::iterator it;
		size_t i =0;
		size_t j=0;
		it = eid_vector.begin();
		//size_t s=eid_vector.size();
		vector<Eid_List*> eid_temp;
		
		for (it=eid_vector.begin();it!=eid_vector.end();++it,i++){
			if (k[j]!=i)
				eid_temp.push_back((*it));
			else {
				delete (*it);
		
				j++;
			     }
	
		}
		/*for (it = eid_canc.begin();it!=eid_canc.end();++it)
				delete (*it);
		*/
		eid_vector=eid_temp;
		//elimino dalla bitmap le sequenze che non sono utili al fine del conteggio delle frequenze delle sequenze, in modo da migliorare
		//le performance dell'algoritmo e l'occupazione di memoria
		//time(&init);
		i=0;
		Bit_vector ris;
		for (size_t f =0;f<listSid.size();++f){
			if (f!=k[i]){
				ris.push_back(listSid[f]);	
			}
			else i++;
		}
		//time(&end);
		//cout<<"ridotta listSid in tempo "<<difftime(end,init)<<endl;
		listSid=ris;

	}
	

	//	void read(const string& s);

	size_t size() {
		return seq->size();
	};

	bool operator<(const CCSMSequence& in);

	bool operator==(const CCSMSequence& in);

	void add(const ItemType& s){
		seq->push_back(s);
		if (seq->size()==1)
			singleton=true;
		else singleton=false;
	}

	std::string toStdString(){
		std::string ris="";
		List_Type::const_iterator it = seq->begin();
		for (;it!=seq->end();++it)
			ris=ris+(*it).asString()+" ";
		return ris;
	}

	void setPresent(size_t sid, bool isHere){
		
		if (sid<listSid.size())
			return;
	//questo serve per riempire l'ultimo elemento del vettore degli
	//Eid_List per quegli elementi che non sono presenti in ultima posizione
		if (sid>listSid.size()){
			for (size_t i=listSid.size();i < sid;i++){
				listSid.push_back(ABSENT);
				eid_vector.push_back(new Eid_List());
			}
		}
		listSid.push_back(isHere);
		eid_vector.push_back(new Eid_List());
		if (isHere)
			count++;
	}

	void setEid(size_t sid, size_t eid){
		//inserisco la posizione dell occorrenza (eventIdentifier)
		//del singleton per
		// questo determinato sid sequenceIdentifier
		std::pair<int,int> couple(eid,eid);
		if (eid_vector.size() == sid)
			eid_vector.push_back(new Eid_List());
		eid_vector[sid]->push_back(couple);
	
	}

	void stampaEid(size_t sid){
		string ris="";
		Eid_List* temp = eid_vector[sid];
		int a, b;
		for (size_t i=0;i<temp->size();++i){
			a=(*temp)[i].first;
			b=(*temp)[i].second;
			cout<<"  ("<<a<<","<<b<<")";
		}
		cout<<endl;
		
	}

	void bitMaptoString(){
		string ris ="";
		for (size_t i=0;i<listSid.size();++i){
			cout<<listSid[i]<<endl;
			stampaEid(i);
		}
	}

	void svuota(){
		delete seq;
		seq = new List_Type();
		}

	~CCSMSequence() {
		delete seq;
		vector<Eid_List*>::iterator it_list;
		for (it_list=eid_vector.begin();it_list!=eid_vector.end();++it_list)
			delete (*it_list);

	};

	int getCount(){
		return count;
	}
	
	bool isSingleton(){
		return singleton;
	}
	/* La sequenza first e una sequenza di lunghezza >=1 mentre la sequenza second è una sequenza di lunghezza = 1
  	 *
	 *
	 */
	static CCSMSequence* merge(CCSMSequence* first, CCSMSequence* second, int min_gap,int max_gap, double threshold);

	static std::vector<size_t> reduce_tr(std::vector<CCSMSequence*>* vec){
		//cout<<"ENTRO NELLA REDUCE_TR"<<endl;
		time_t init,end;
		vector<size_t> k;
		vector<CCSMSequence*>::iterator it;
		Bit_vector ris;
		if (vec->size()==0)
			return k;
		it=vec->begin();
		//cout<<"ecco le sequenze su cui applico la reduce"<<endl;
		//cout<<(*it)->toStdString()<<endl;
		ris=(*it)->listSid;
		
		++it;
		time(&init);
		for (;it!=vec->end();++it){
			//cout<<(*it)->toStdString()<<endl;
			ris=ris|(*it)->listSid;
		}
		time(&end);
		//cout<<"----------"<<vec->size()<<"------"<<difftime(end,init)<<"----------------------"<<endl;
		Bit_vector::iterator b_it;
		b_it=ris.begin();
		size_t i =0;
		
		time(&init);
		for (;b_it!=ris.end();++b_it,++i){
			if (!(*b_it))
				k.push_back(i);
		}
		time(&end);
		//cout<<"----------secondo ciclo interno------"<<difftime(end,init)<<"----------------------"<<endl;
		return k;

	}

	ItemType getFirstItem(){
		List_Type::iterator it=seq->begin();
		return (*it);
	}

	CCSMSequence* getLastItem(){
		return last;
	}

	void setLastItem(CCSMSequence* in){
		last=in;
	}

	ResultItems getSequenceItems(){
		ResultItems ris(*seq,count);
		return ris;
	}

	// inserisco nella lista le sequenze di cui questa e il prefisso
	//
	void addPrefixSequence(CCSMSequence* in){
		prefix.push_back(in);
	}

	list<CCSMSequence*> getPrefixSequence(){
		return prefix;
	}

	// inserisco nella lista le sequence di cui questa e il suffisso
	//
	void addSuffixSequence(CCSMSequence* in){
		suffix.push_back(in);
	}

	list<CCSMSequence*> getSuffixSequence(){
		return suffix;
	}

	//da questa sequenza lunga k-2 puo essere contenuta in una possibile sequenza frequente di lunghezza k
	//se ha almeno un elemento nella Suffix list e uno nella Prefix list
	bool canGenerate(){
		return suffix.size()>0 && prefix.size()>0; 
	}
	
	void read(odbc::ResultSet* rs, const HeadBodySourceRowDescription& rowDes);

};


}//END NAMESPACE

#endif

